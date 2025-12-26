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

EWRAM_DATA ALIGN(4) uint8_t ScratchPad[240 * 160 * 3]; // scratch pad memory for decompression

auto Decompress_LZ4(const uint8_t *data) -> void
{
	Compression::LZ4UnCompWrite8bit_ASM(reinterpret_cast<const uint32_t *>(data), reinterpret_cast<uint32_t *>(ScratchPad));
}

auto Decompress_LZ10(const uint8_t *data) -> void
{
	Compression::LZ77UnCompWrite16bit_ASM(data, ScratchPad);
}

struct ImageEntry
{
	const char *codec = nullptr;
	const uint8_t *data = nullptr;
	void (*func)(const uint8_t *) = nullptr;
};

const ImageEntry Images[] = {
	{"             LZ4", IMAGE_LZ4_DATA, &Decompress_LZ4},
	{"         LZSS / LZ10", IMAGE_LZ10_DATA, &Decompress_LZ10},
};

int main()
{
	// set waitstates for GamePak ROM and EWRAM
	Memory::RegWaitCnt = Memory::WaitCntFast;
	Memory::RegWaitEwram = Memory::WaitEwramNormal;
	// start wall clock
	irqInit();
	uint32_t imageIndex = 0;
	do
	{
		// set up text UI
		TUI::setup();
		TUI::fillBackground(TUI::Color::Black);
		TUI::printf(0, 0, "      Decompression demo");
		TUI::printf(0, 9, "    Press A to decompress");
		TUI::printf(0, 11, Images[imageIndex].codec);
		// wait for keypress
		do
		{
			scanKeys();
			if (keysDown() & KEY_A)
			{
				break;
			}
		} while (true);
		// switch video mode to 240x160x2
		REG_DISPCNT = MODE_3 | BG2_ON;
		// start benchmark timer
		REG_TM3CNT_L = 0;
		REG_TM3CNT_H = TIMER_START | 2;
		// decode and blit image
		Images[imageIndex].func(Images[imageIndex].data);
		// end benchmark timer
		REG_TM3CNT_H = 0;
		auto durationMs = static_cast<int32_t>(REG_TM3CNT_L) * 1000;
		Debug::printf("Decoding time: %f ms", durationMs);
		// convert image from RGB888 to BGR555 and display
		auto srcPtr8 = reinterpret_cast<const uint8_t *>(ScratchPad);
		auto dstPtr16 = reinterpret_cast<uint16_t *>(VRAM);
		for (uint32_t i = 0; i < 240 * 160; ++i, srcPtr8 += 3)
		{
			uint32_t pixel = static_cast<uint32_t>(srcPtr8[0]) >> 3;
			pixel |= (static_cast<uint32_t>(srcPtr8[1]) >> 3) << 5;
			pixel |= (static_cast<uint32_t>(srcPtr8[2]) >> 3) << 10;
			*dstPtr16++ = pixel;
		}
		// wait for keypress
		do
		{
			scanKeys();
			if (keysDown() & KEY_A)
			{
				break;
			}
		} while (true);
		imageIndex = (imageIndex + 1) % (sizeof(Images) / sizeof(ImageEntry));
	} while (true);
	return 0;
}
