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

Options::Option Options::toStdout{
    false,
    {"stdout", "Write output data to stdout.", cxxopts::value(toStdout.isSet)}};

Options::OptionT<std::string> Options::toSrc{
    false,
    {"tosrc", "Write output data to OUTNAME.h and OUTNAME.c files. Variable will start with \"OUTNAME_\".", cxxopts::value(toSrc.value)},
    "",
    {},
    [](const cxxopts::ParseResult &r)
    {
        if (r.count(toSrc.cxxOption.opts_))
        {
            toSrc.isSet = true;
        }
    }};

Options::Option Options::decompress{
    false,
    {"decompress", "Decompress input data.", cxxopts::value(decompress.isSet)}};

Options::Option Options::force{
    false,
    {"force", "Force overwriting of output file(s).", cxxopts::value(decompress.isSet)}};

Options::Option Options::lz4{
    false,
    {"lz4", "Use LZ4 compression variant 40h.", cxxopts::value(lz4.isSet)}};

Options::Option Options::lz10{
    false,
    {"lz10", "Use LZ77 compression variant 10h (GBA BIOS decompressable).", cxxopts::value(lz10.isSet)}};

/*Options::Option Options::rle{
    false,
    {"rle", "Use RLE compression.", cxxopts::value(rle.isSet)}};*/

Options::Option Options::vram{
    false,
    {"vram", "Make compression VRAM-safe if possible.", cxxopts::value(vram.isSet)}};

Options::Option Options::dryRun{
    false,
    {"dryrun", "Process data, but do not write output files.", cxxopts::value(dryRun.isSet)}};
