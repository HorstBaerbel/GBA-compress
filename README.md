# GBA-compress

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

Game Boy Advance tools for compression:

* [gbacomp](src/gbacomp.cpp) - Compress a file to a .h / .c file to compile them into your program or output data to stdout.
  It compresses data to LZSS / LZ77 / LZ10 GBA BIOS-compatible data or to LZ4 (better compression, faster decompression). 
* The [gba](gba) subfolder contains a GBA example for decompressing that data using fast ARM assembler functions.

The frame format of the compressed data is analog to the GBA BIOS decompression functions documented [here](https://problemkaputt.de/gbatek.htm#biosdecompressionfunctions):  

* First a data header (32bit):  
  - Bit 0-3   Reserved (0)  
  - Bit 4-7   Compressed type (must be 1 for LZ77, 4 for LZ4)  
  - Bit 8-31  Size of decompressed data  
* Then follows the compressed data

If you find a bug or make an improvement your pull requests are appreciated.

## License

All of this is under the [MIT License](LICENSE). It uses:

* [cxxopts](https://github.com/jarro2783/cxxopts) for command line argument parsing.
* [glob](https://github.com/p-ranav/glob) for input file globbing / wildcards.
* [3ds-cmake](https://github.com/Xtansia/3ds-cmake) for building GBA binaries.
* [Catch2](https://github.com/catchorg/Catch2) for unit tests.
* [LZ77 decompression routines](gba/compression/lz77.s) from [Lorenzooone/Pokemon-Gen3-to-Gen-X](https://github.com/Lorenzooone/Pokemon-Gen3-to-Gen-X/blob/main/source/decompress.s) which is in turn derived from [Cult-of-GBA/BIOS](https://github.com/Cult-of-GBA/BIOS/blob/master/bios_calls/decompression/lz77.s). Licensed as [MIT License](https://github.com/Cult-of-GBA/BIOS/blob/master/LICENSE).
* Test images and video from the Blender "[Big Buck Bunny](https://peach.blender.org/)" movie (c) copyright 2008, Blender Foundation. Licensed as [CC BY 3.0](https://creativecommons.org/licenses/by/3.0/).
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
