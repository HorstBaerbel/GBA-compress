#pragma once

#include "exception.h"

#include <cstdint>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

namespace IO
{

    class Text
    {
    public:
        /// @brief Write data information to a .h file.
        static auto writeDataToH(std::ofstream &hFile, const std::string &varName, const std::vector<uint32_t> &data, std::size_t actualDataSize) -> void;

        /// @brief Write data to a .c file.
        static auto writeDataToC(std::ofstream &cFile, const std::string &varName, const std::string &hFileBaseName, const std::vector<uint32_t> &data) -> void;
    };

}
