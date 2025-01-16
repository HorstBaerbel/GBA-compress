#include "options.h"

#include "exception.h"

Options::Option::operator bool() const
{
    return isSet;
}

std::string Options::Option::helpString() const
{
    return cxxOption.opts_ + ": " + cxxOption.desc_;
}

Options::Option Options::lz10{
    false,
    {"lz10", "Use LZ77 compression variant 10.", cxxopts::value(lz10.isSet)}};

/*Options::Option Options::rle{
    false,
    {"rle", "Use RLE compression.", cxxopts::value(rle.isSet)}};*/

Options::Option Options::vram{
    false,
    {"vram", "Make compression VRAM-safe if possible.", cxxopts::value(vram.isSet)}};

Options::Option Options::dryRun{
    false,
    {"dryrun", "Process data, but do not write output files.", cxxopts::value(dryRun.isSet)}};
