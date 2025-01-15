# GBA-compress

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT) ![Build](https://github.com/HorstBaerbel/gba-image-tools/workflows/Build/badge.svg)

Game Boy Advance tools for compression:

* [gbacomp](src/gbacomp.cpp) - Convert / compress a (list of) file(s) to a .h / .c file to compile them into your program or output data to stdout.

If you find a bug or make an improvement your pull requests are appreciated.

## License

All of this is under the [MIT License](LICENSE). It uses:

* [cxxopts](https://github.com/jarro2783/cxxopts) for command line argument parsing.
* [glob](https://github.com/p-ranav/glob) for input file globbing / wildcards.
* [3ds-cmake](https://github.com/Xtansia/3ds-cmake) for building GBA binaries.
* [Catch2](https://github.com/catchorg/Catch2) for unit tests.
* [LZ77 decompression routines](gba/compression/lz77.s) from [Lorenzooone/Pokemon-Gen3-to-Gen-X](https://github.com/Lorenzooone/Pokemon-Gen3-to-Gen-X/blob/main/source/decompress.s) which is in turn derived from [Cult-of-GBA/BIOS](https://github.com/Cult-of-GBA/BIOS/blob/master/bios_calls/decompression/lz77.s). Licensed as [MIT License](https://github.com/Cult-of-GBA/BIOS/blob/master/LICENSE).

## Prequisites

* You **must** have [OpenMP](https://www.openmp.org/) installed for compiling. Install it with:

  ```apt install libomp-dev``` or ```dnf install libomp-devel```

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
