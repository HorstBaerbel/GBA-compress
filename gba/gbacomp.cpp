#include "compression/lz4.h"
#include "compression/lz77.h"
#include "memory/memory.h"
#include "print/output.h"
#include "sys/base.h"
#include "sys/input.h"
#include "sys/interrupts.h"
#include "sys/timers.h"
#include "sys/video.h"
#include "tui.h"

#include "data/images.h"

EWRAM_DATA ALIGN(4) uint8_t ScratchPad[240 * 160 * 2];

auto waitForKey(KEYPAD_BITS key) -> void
{
	do
	{
		scanKeys();
		if (keysDown() & key)
		{
			break;
		}
	} while (true);
}

auto Decompress_LZ4(const uint8_t *const *compressedData, uint32_t compressedCount) -> void
{
	for (uint32_t i = 0; i < compressedCount; ++i)
	{
		Compression::LZ4UnCompWrite16bit_ASM(reinterpret_cast<const uint32_t *>(compressedData[i]), reinterpret_cast<uint32_t *>(VRAM));
	}
}

auto Decompress_LZ77(const uint8_t *const *compressedData, uint32_t compressedCount) -> void
{
	for (uint32_t i = 0; i < compressedCount; ++i)
	{
		Compression::LZ77UnCompWrite16bit_ASM(compressedData[i], reinterpret_cast<uint32_t *>(VRAM));
	}
}

struct CodecEntry
{
	const char *name = nullptr;
	void (*func)(const uint8_t *const *, uint32_t) = nullptr;
	uint32_t compressedCount = 0;
	const uint8_t *compressedData[8];
};

static const CodecEntry Codecs[] = {
	{"             LZ4", &Decompress_LZ4, 8, {IMAGE_LZ4_0_DATA, IMAGE_LZ4_1_DATA, IMAGE_LZ4_2_DATA, IMAGE_LZ4_3_DATA, IMAGE_LZ4_4_DATA, IMAGE_LZ4_5_DATA, IMAGE_LZ4_6_DATA, IMAGE_LZ4_7_DATA}},
	{"         LZSS / LZ77", &Decompress_LZ77, 8, {IMAGE_LZ77_0_DATA, IMAGE_LZ77_1_DATA, IMAGE_LZ77_2_DATA, IMAGE_LZ77_3_DATA, IMAGE_LZ77_4_DATA, IMAGE_LZ77_5_DATA, IMAGE_LZ77_6_DATA, IMAGE_LZ77_7_DATA}}};

auto Compare_LZ4_LZ77() -> bool
{
	bool success = true;
	const auto compressedCount = Codecs[0].compressedCount < Codecs[1].compressedCount ? Codecs[0].compressedCount : Codecs[1].compressedCount;
	for (uint32_t ci = 0; ci < compressedCount; ++ci)
	{
		Compression::LZ4UnCompWrite16bit_ASM(reinterpret_cast<const uint32_t *>(Codecs[0].compressedData[ci]), reinterpret_cast<uint32_t *>(VRAM));
		Compression::LZ77UnCompWrite16bit_ASM(Codecs[1].compressedData[ci], ScratchPad);
		for (uint32_t i = 0; i < sizeof(ScratchPad) / 2; ++i)
		{
			if (reinterpret_cast<const uint16_t *>(VRAM)[i] != reinterpret_cast<const uint16_t *>(ScratchPad)[i])
			{
				success = false;
				Debug::printf("Uncompressed data %d difference in pixel %d", ci, i);
				break;
			}
		}
	}
	return success;
}

int main()
{
	// set waitstates for GamePak ROM and EWRAM
	Memory::RegWaitCnt = Memory::WaitCntFast;
	Memory::RegWaitEwram = Memory::WaitEwramNormal;
	// start wall clock
	irqInit();
	// set up text UI
	TUI::setup();
	TUI::fillBackground(TUI::Color::Black);
	TUI::fillForeground(TUI::Color::Black);
	TUI::printf(0, 0, "      Decompression demo");
	TUI::printf(0, 11, "Press A to check decompression");
	// wait for keypress
	waitForKey(KEY_A);
	// switch video mode to 240x160x2
	REG_DISPCNT = MODE_3 | BG2_ON;
	// first check if we're decoding LZ4 / LZ77 correctly
	const auto success = Compare_LZ4_LZ77();
	TUI::setup();
	TUI::fillBackground(TUI::Color::Black);
	TUI::fillForeground(TUI::Color::Black);
	TUI::printf(0, 0, "      Decompression demo");
	TUI::printf(0, 9, "Press A to skip to benchmarks");
	TUI::setColor(TUI::Color::Black, success ? TUI::Color::LightGreen : TUI::Color::LightRed);
	TUI::printf(0, 11, success ? "       Decompression ok" : "     Decompression failed");
	// wait for keypress
	waitForKey(KEY_A);
	uint32_t codecIndex = 0;
	do
	{
		const auto &codec = Codecs[codecIndex];
		// set up text UI
		TUI::setup();
		TUI::fillBackground(TUI::Color::Black);
		TUI::fillForeground(TUI::Color::Black);
		TUI::printf(0, 0, "      Decompression demo");
		TUI::printf(0, 9, codec.name);
		TUI::printf(0, 11, "    Press A to decompress");
		// wait for keypress
		waitForKey(KEY_A);
		// switch video mode to 240x160x2
		REG_DISPCNT = MODE_3 | BG2_ON;
		// start benchmark timer
		REG_TM3CNT_L = 0;
		REG_TM3CNT_H = TIMER_START | 2;
		// run test function
		codec.func(codec.compressedData, codec.compressedCount);
		// end benchmark timer
		REG_TM3CNT_H = 0;
		auto durationMs = static_cast<int32_t>(REG_TM3CNT_L) * 1000;
		TUI::setup();
		TUI::fillBackground(TUI::Color::Black);
		TUI::fillForeground(TUI::Color::Black);
		TUI::printf(0, 0, "      Decompression demo");
		TUI::printf(0, 9, codec.name);
		TUI::printf(0, 11, "  Decompressed in: %f ms", durationMs);
		// wait for keypress
		waitForKey(KEY_A);
		codecIndex = (codecIndex + 1) % (sizeof(Codecs) / sizeof(CodecEntry));
	} while (true);
	return 0;
}
