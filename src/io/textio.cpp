#include "textio.h"

#include <filesystem>
#include <iostream>

namespace IO
{

    /// @brief Write values as a comma-separated array of hex numbers.
    template <typename T>
    static auto writeValues(std::ofstream &outFile, const std::vector<T> &data, bool asHex = false) -> void
    {
        auto flags = outFile.flags();
        size_t loopCount = 0;
        for (auto current : data)
        {
            if (asHex)
            {
                outFile << "0x" << std::hex << std::noshowbase << std::setw(sizeof(T) * 2) << std::setfill('0') << current;
            }
            else
            {
                outFile << std::dec << current;
            }
            if (loopCount < data.size() - 1)
            {
                outFile << ", ";
            }
            if (++loopCount % 10 == 0)
            {
                outFile << std::endl;
            }
        }
        outFile.flags(flags);
    }

    auto Text::writeDataToH(std::ofstream &hFile, const std::string &varName, const std::vector<uint32_t> &data, std::size_t actualDataSize) -> void
    {
        hFile << "#pragma once" << std::endl;
        hFile << "#include <stdint.h>" << std::endl
              << std::endl;
        hFile << "#define " << varName << "_DATA_SIZE " << actualDataSize << " // actual size of data in bytes" << std::endl;
        hFile << "#define " << varName << "_BLOB_SIZE " << data.size() << " // size of data blob in 4 byte units (= DATA_SIZE + padding)" << std::endl;
        hFile << "extern const uint32_t " << varName << "_DATA[" << varName << "_BLOB_SIZE];" << std::endl;
    }

    auto Text::writeDataToC(std::ofstream &cFile, const std::string &varName, const std::string &hFileBaseName, const std::vector<uint32_t> &data) -> void
    {
        cFile << "#include \"" << hFileBaseName << ".h\"" << std::endl
              << std::endl;
        cFile << "const _Alignas(4) uint32_t " << varName << "_DATA[" << varName << "_BLOB_SIZE] = { " << std::endl;
        writeValues(cFile, data, true);
        cFile << "};" << std::endl;
    }
}
