#include "compression/lzss.h"
#include "exception.h"
#include "io/textio.h"
#include "options.h"

#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <string>

#include "cxxopts/include/cxxopts.hpp"
#include "glob/single_include/glob/glob.hpp"

std::string m_inFile;
std::string m_outFile;
Options options;

std::string getCommandLine(int argc, const char *argv[])
{
    std::string result;
    for (int i = 1; i < argc; i++)
    {
        result += std::string(argv[i]);
        if (i < (argc - 1))
        {
            result += " ";
        }
    }
    return result;
}

bool readArguments(int argc, const char *argv[])
{
    try
    {
        cxxopts::Options opts("img2h", "Convert and compress a list images to a .h / .c file to compile it into a program");
        opts.add_option("", {"h,help", "Print help"});
        opts.add_option("", {"infile", "Input file, use \"-\" for stdin", cxxopts::value<std::string>()});
        opts.add_option("", {"outname", "Output file and variable name, e.g \"foo\". This will name the output files \"foo.h\" and \"foo.c\" and variable names will start with \"FOO_\", use \"-\" for stdin", cxxopts::value<std::string>()});
        opts.add_option("", options.lz10.cxxOption);
        opts.add_option("", options.vram.cxxOption);
        opts.add_option("", options.dryRun.cxxOption);
        opts.parse_positional({"infile", "outname"});
        auto result = opts.parse(argc, argv);
        // check if help was requested
        if (result.count("h"))
        {
            return false;
        }
        // get output file / name
        if (result.count("outname"))
        {
            m_outFile = result["outname"].as<std::string>();
        }
        else
        {
            std::cout << "No output passed!" << std::endl;
            return false;
        }
        // get input file(s)
        if (result.count("infile"))
        {
            m_inFile = result["infile"].as<std::string>();
            // make sure input file exists
            if (!std::filesystem::exists(m_inFile))
            {
                std::cout << "Input file \"" << m_inFile << "\" does not exist!" << std::endl;
                return false;
            }
        }
        else
        {
            std::cout << "No input passed!" << std::endl;
            return false;
        }
    }
    catch (const cxxopts::exceptions::parsing &e)
    {
        std::cerr << "In command line: " << getCommandLine(argc, argv) << std::endl;
        std::cerr << "Argument error: " << e.what() << std::endl;
        return false;
    }
    return true;
}

void printUsage()
{
    // 80 chars:  --------------------------------------------------------------------------------
    std::cout << "Compress and convert a file to a .c and .h file to compile into a" << std::endl;
    std::cout << "GBA executable or output data to stdout." << std::endl;
    std::cout << "Usage: gbacomp METHOD INFILE [INFILEn...] OUTNAME" << std::endl;
    std::cout << "Compression METHODs (mutually exclusive):" << std::endl;
    std::cout << options.lz10.helpString() << std::endl;
    std::cout << "COMPRESS modifiers (optional):" << std::endl;
    std::cout << options.vram.helpString() << std::endl;
    std::cout << "Valid combinations are e.g. \"--rle --lz10\"." << std::endl;
    std::cout << "INFILE: can be a file list and/or can have * as a wildcard." << std::endl;
    std::cout << "OUTNAME: is determined from the first non-existant file path. It can be an " << std::endl;
    std::cout << "absolute or relative file path or a file base name. Two files OUTNAME.h and " << std::endl;
    std::cout << "OUTNAME.c will be generated. All variables will begin with the base name " << std::endl;
    std::cout << "portion of OUTNAME." << std::endl;
    std::cout << "MISC options (all optional):" << std::endl;
    std::cout << options.dryRun.helpString() << std::endl;
    std::cout << "help: Show this help." << std::endl;
}

int main(int argc, const char *argv[])
{
    try
    {
        // check arguments
        if (argc < 3 || !readArguments(argc, argv))
        {
            printUsage();
            return 2;
        }
        // check input and output
        if (m_inFile.empty())
        {
            std::cerr << "No input passed. Aborting." << std::endl;
            return 1;
        }
        if (m_outFile.empty())
        {
            std::cerr << "No output passed. Aborting." << std::endl;
            return 1;
        }
        const bool writeToStdout = m_outFile == "-";
        // read from stdin or file depending on option
        std::vector<uint8_t> inData;
        const bool readFromStdin = m_inFile == "-";
        if (readFromStdin)
        {
            // reopen stdin in binary mode
            freopen(nullptr, "rb", stdin);
            // read from stdin until EOF
            while (!feof(stdin) && !ferror(stdin))
            {
                std::vector<uint8_t> block(1024, 0);
                auto bytesRead = fread(block.data(), 1, block.size(), stdin);
                std::copy(block.cbegin(), std::next(block.cbegin(), bytesRead), std::back_inserter(inData));
            }
            freopen(nullptr, "r", stdin);
        }
        else
        {
            // read from input file
            auto inFileSize = std::filesystem::file_size(std::filesystem::path(m_inFile));
            REQUIRE(inFileSize != 0, std::runtime_error, "Input file " << m_inFile << " is empty");
            std::ifstream inStream(m_inFile, std::ios::binary);
            REQUIRE(inStream.is_open(), std::runtime_error, "Failed to open " << m_inFile << " for reading");
            inData = std::vector<uint8_t>(inFileSize);
            inStream.read(reinterpret_cast<char *>(inData.data()), inFileSize);
            inStream.close();
        }
        if (!writeToStdout)
        {
            std::cout << "Read " << inData.size() << " bytes from " << (readFromStdin ? "stdin" : m_inFile) << std::endl;
        }
        // compress data
        std::vector<uint8_t> outData;
        if (options.lz10)
        {
            outData = Compression::encodeLZ10(inData, {options.vram.isSet});
        }
        if (!writeToStdout)
        {
            std::cout << "Data compressed to " << outData.size() << " bytes (" << (static_cast<double>(outData.size()) / static_cast<double>(inData.size()) * 100.0) << "%)" << std::endl;
        }
        // open output files / stdout
        if (!options.dryRun)
        {
            if (writeToStdout)
            {
                // reopen stdout in binary mode
                freopen(nullptr, "wb", stdout);
                // write to stdout
                auto bytesWritten = fwrite(outData.data(), 1, outData.size(), stdout);
                freopen(nullptr, "w", stdout);
                REQUIRE(bytesWritten == outData.size(), std::runtime_error, "Failed to write data to stdout");
            }
            else
            {
                std::ofstream hFile(m_outFile + ".h", std::ios::out);
                std::ofstream cFile(m_outFile + ".c", std::ios::out);
                if (hFile.is_open() && cFile.is_open())
                {
                    std::cout << "Writing output files " << m_outFile << ".h, " << m_outFile << ".c" << std::endl;
                    try
                    {
                        // build output file / variable name
                        std::string baseName = std::filesystem::path(m_outFile).filename().replace_extension("");
                        std::string varName = baseName;
                        std::transform(varName.begin(), varName.end(), varName.begin(), [](char c)
                                       { return std::toupper(c, std::locale()); });
                        // output header
                        hFile << "// Converted with gbacomp " << getCommandLine(argc, argv) << std::endl;
                        hFile << "// Note that the _Alignas specifier will need C11, as a workaround use __attribute__((aligned(4)))" << std::endl
                              << std::endl
                              << std::endl;
                        // padd data to 32-bit
                        const auto actualDataSize = outData.size();
                        if (outData.size() % sizeof(uint32_t) != 0)
                        {
                            auto newSize = ((outData.size() + sizeof(uint32_t) - 1) / sizeof(uint32_t)) * sizeof(uint32_t);
                            outData.resize(newSize);
                        }
                        // convert data to 32-bit values
                        std::vector<uint32_t> outData32(outData.size() / sizeof(uint32_t));
                        std::memcpy(outData32.data(), outData.data(), outData.size());
                        // output image and palette data
                        IO::Text::writeDataToH(hFile, varName, outData32, actualDataSize);
                        IO::Text::writeDataToC(cFile, varName, baseName, outData32);
                        hFile << std::endl;
                        hFile.close();
                        cFile.close();
                    }
                    catch (const std::runtime_error &e)
                    {
                        hFile.close();
                        cFile.close();
                        std::cerr << "Failed to write data to output files: " << e.what() << std::endl;
                        return 1;
                    }
                }
                else
                {
                    hFile.close();
                    cFile.close();
                    std::cerr << "Failed to open " << m_outFile << ".h, " << m_outFile << ".c for writing" << std::endl;
                    return 1;
                }
            }
        }
        std::cout << "Done" << std::endl;
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}