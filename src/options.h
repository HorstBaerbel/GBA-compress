#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <cxxopts/include/cxxopts.hpp>

class Options
{
public:
    struct Option
    {
        bool isSet = false;
        cxxopts::Option cxxOption;

        operator bool() const;

        std::string helpString() const;
    };

    template <typename T>
    struct OptionT : Option
    {
        T value;
        std::string valueString;
        std::function<void(const cxxopts::ParseResult &)> parse;
    };

    static Option toStdout;
    static OptionT<std::string> toSrc;
    static Option decompress;
    static Option force;
    static Option lz4;
    static Option lz10;
    // static Option rle;
    static Option vram;
    static Option dryRun;
};
