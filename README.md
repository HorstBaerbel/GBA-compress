# GBA-compress

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

Game Boy Advance tools for data compression:

* [gbacomp](src/gbacomp.cpp) - Compress a file to a .h / .c file to compile them into your program, to a binary file to include via a .s file, or outputs compressed data to stdout. The interface is similar to [gbalzss](https://github.com/devkitPro/gba-tools).
  It compresses data to [LZSS / LZ77 / LZ10 GBA BIOS-compatible data](https://problemkaputt.de/gbatek.htm#biosdecompressionfunctions) or to [LZ4](https://fastcompression.blogspot.com/2011/05/lz4-explained.html) (better compression, faster decompression). 
* The [gba](gba) subfolder contains a GBA example for decompressing that data using fast ARM assembler functions.

If you find a bug or make an improvement your pull requests are appreciated.

## Data format

The frame format of the compressed data is analogous to the GBA BIOS decompression functions documented [here](https://problemkaputt.de/gbatek.htm#biosdecompressionfunctions):  

* First a data header (32 bit):
  - Bit 0-3   Reserved (0)
  - Bit 4-7   Compressed type (must be 1 for LZ77 / LZ10, 4 for LZ4)
  - Bit 8-31  Size of decompressed data
* Then follows the compressed data

## Compression / Speed

### Compression ratio

Please be aware that my compression data "corpus" is just some files I threw together to match my use-case, so **take results with a grain of salt**. You can find the files in the [data](/data) folder and the ratio and roundtrip tests in [test](/test).

| File                             | Content                   | Size   | LZ4 size   | LZ77 size^ |
| -------------------------------- | ------------------------- | ------ | ---------- | ---------- |
| lorem_ipsum_2k.txt               | Text                      | 2048   | **792**    | 852        |
| artificial_240x160.raw           | RGB888 artificial image   | 115200 | **59480**  | 63860      |
| flower_foveon_240x160_bgr555.raw | XBGR1555 photo image      | 76800  | 34812      | **33136**  |
| BigBuckBunny_40_240x160.raw      | RGB888 mostly white image | 115200 | **10768**  | 20564      |
| black_240x160.raw                | RGB888 all black image    | 115200 | **1040**   | 13616      |
| mask6.raw                        | Bit mask                  | 4096   | **76**     | 516        |
| squish_240x160.raw               | RGB888 gradient image     | 115200 | **61412**  | 72488      |
| bbb_adpcm_22050.wav              | ADPCM audio data          | 384732 | **285560** | 316932     |

^: For LZ77 gbacomp usually matches or outperforms [gbalzss](https://github.com/devkitPro/gba-tools) due to [optimal parsing](https://create.stephan-brumme.com/smallz4) of the data.

### Decompression speed on GBA

There is a GBA example in [gba/gbacomp.cpp](/gba/gbacomp.cpp) that tests and benchmarks decompression on the GBA. It uses 8 XBGR1555 images, which (see table above) is a worst-case scenario for LZ4 ratio wise. gbacomp shrinks these files to **226.3 kB** using LZ4 and 238.7 kB using LZ77, so only a small win for LZ4. Decompression on GBA takes **346.1 ms** for LZ4 and 404.6 ms for LZ77, so a clear win for LZ4 there. There's an option to use DMA3 to decompress LZ4, which is even faster (but not everybody might like to use DMA for this).

|                           | LZ4 (DMA) | LZ4             | LZ77^           |
| ------------------------- | --------- | --------------- | --------------- |
| Size / kB                 | **226.3** | **226.3**       | 238.7 (+5.5 %)  |
| Decompression / ms (mGBA) | **282.3** | 346.1 (+22.6 %) | 404.6 (+43.3 %) |

^: The LZ77 decompression routine is from [Lorenzooone/Pokemon-Gen3-to-Gen-X](https://github.com/Lorenzooone/Pokemon-Gen3-to-Gen-X/blob/main/source/decompress.s), which is the fastest routine I know of and definitely faster than the standard BIOS routine.

## License

All of this is under the [MIT License](LICENSE). It uses:

* [cxxopts](https://github.com/jarro2783/cxxopts) for command line argument parsing.
* [glob](https://github.com/p-ranav/glob) for input file globbing / wildcards.
* [3ds-cmake](https://github.com/Xtansia/3ds-cmake) for building GBA binaries.
* [Catch2](https://github.com/catchorg/Catch2) for unit tests.
* The [LZ77 decompression routine](gba/compression/lz77.s) from [Lorenzooone/Pokemon-Gen3-to-Gen-X](https://github.com/Lorenzooone/Pokemon-Gen3-to-Gen-X/blob/main/source/decompress.s) which is in turn derived from [Cult-of-GBA/BIOS](https://github.com/Cult-of-GBA/BIOS/blob/master/bios_calls/decompression/lz77.s). Licensed as [MIT License](https://github.com/Cult-of-GBA/BIOS/blob/master/LICENSE).
* Test images from the Blender "[Big Buck Bunny](https://peach.blender.org/)" movie (c) copyright 2008, Blender Foundation. Licensed as [CC BY 3.0](https://creativecommons.org/licenses/by/3.0/).
* Test images from [imagecompression.info](https://imagecompression.info/test_images/) resized to fit my needs.

## Prequisites

* You **must** have [OpenMP](https://www.openmp.org/) installed for compiling. Install it with:

  ```apt install libomp-dev``` or ```dnf install libomp-devel```

* The GBA examples **need** to have [devkitPro / devkitArm](https://devkitpro.org/) installed for compiling. Installation instructions can be found [here](https://devkitpro.org/wiki/Getting_Started).

## Building

### From the command line

Navigate to the GBA-compress folder, then:

```sh
mkdir build && cd build
cmake ..
make
```

To build a release package, call:

```sh
make package
```

### From Visual Studio Code

* **Must**: Install the "C/C++ extension" by Microsoft.
* **Must**: Install the "CMake Tools" extension by Microsoft.
* You might need to restart / reload Visual Studio Code if you have installed extensions.
* Open the GBA-compress folder using "Open folder...".
* Choose a kit of your choice as your active CMake kit if asked.
* You should be able to build now using F7 and build + run using F5.
