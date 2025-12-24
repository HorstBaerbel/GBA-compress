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
	TUI::printf(0, 8, "      Decompression demo");
	TUI::printf(0, 10, "       Press A to skip");
	TUI::printf(0, 11, "        to next image");
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
	do
	{
		// start benchmark timer
		REG_TM3CNT_L = 0;
		REG_TM3CNT_H = TIMER_START | 2;
		// decode and blit image
		Compression::LZ77UnCompWrite16bit(IMAGE_LZ10_DATA, ScratchPad);
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
		// wait for next key press
		do
		{
			scanKeys();
			if (keysDown() & KEY_A)
			{
				break;
			}
		} while (true);
	} while (true);
	return 0;
}
