#include "testmacros.h"

#include "compression/lzss.h"
#include "exception.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct TestFile
{
    std::string fileName;
    std::size_t maxSize;
};

const std::vector<TestFile> TestFiles = {
    {"lorem_ipsum_2k.txt", 852},
    {"artificial_240x160.raw", 63860},
    {"BigBuckBunny_40_240x160.raw", 20564},
    {"black_240x160.raw", 13616},
    {"mask6.raw", 516},
    {"squish_240x160.raw", 72488}};

const std::string DataPath = "../../data/data/";

using namespace Compression;

TEST_SUITE("LZSS")

/*void findDiff(const std::vector<uint8_t> &a, const std::vector<uint8_t> &b)
{
    auto aIt = a.cbegin();
    auto bIt = b.cbegin();
    while (aIt != a.cend() && bIt != b.cend() && *aIt == *bIt)
    {
        ++aIt;
        ++bIt;
    }
    if (aIt == a.cend() && bIt == b.cend())
    {
        return;
    }
    std::cout << "Difference at index " << std::distance(a.cbegin(), aIt) << ": ";
    if (aIt != a.cend())
    {
        std::cout << "0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<uint32_t>(*aIt) << " ";
    }
    else
    {
        std::cout << "???? ";
    }
    if (bIt != b.cend())
    {
        std::cout << "0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<uint32_t>(*bIt) << " ";
    }
    else
    {
        std::cout << "???? ";
    }
}*/

    // Basic tests
static const uint8_t v0[] = {};
static const uint8_t v1[] = {0};
static const uint8_t v2[] = {0, 0, 0, 0};
static const uint8_t v3[] = {1, 2, 3, 4};
static const uint8_t v4[] = {1, 2, 3, 4, 2, 3, 4};
// Lorem ipsum string
static const uint8_t v5[] = "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.";
// Lorem ipsum string encoded by gbalzss (This has a different encoding, still we should be able to correctly decode it)
static const uint8_t v5_gbalzss[] = {0x10, 0x50, 0x02, 0x00, 0x00, 0x4C, 0x6F, 0x72, 0x65, 0x6D, 0x20, 0x69, 0x70, 0x00, 0x73, 0x75,
                                     0x6D, 0x20, 0x64, 0x6F, 0x6C, 0x6F, 0x00, 0x72, 0x20, 0x73, 0x69, 0x74, 0x20, 0x61, 0x6D, 0x00,
                                     0x65, 0x74, 0x2C, 0x20, 0x63, 0x6F, 0x6E, 0x73, 0x04, 0x65, 0x74, 0x65, 0x74, 0x75, 0x00, 0x14,
                                     0x61, 0x64, 0x80, 0x00, 0x23, 0x63, 0x69, 0x6E, 0x67, 0x20, 0x65, 0x6C, 0x00, 0x69, 0x74, 0x72,
                                     0x2C, 0x20, 0x73, 0x65, 0x64, 0x00, 0x20, 0x64, 0x69, 0x61, 0x6D, 0x20, 0x6E, 0x6F, 0x00, 0x6E,
                                     0x75, 0x6D, 0x79, 0x20, 0x65, 0x69, 0x72, 0x00, 0x6D, 0x6F, 0x64, 0x20, 0x74, 0x65, 0x6D, 0x70,
                                     0x80, 0x00, 0x44, 0x69, 0x6E, 0x76, 0x69, 0x64, 0x75, 0x6E, 0x00, 0x74, 0x20, 0x75, 0x74, 0x20,
                                     0x6C, 0x61, 0x62, 0x88, 0x00, 0x64, 0x20, 0x65, 0x74, 0x30, 0x60, 0x65, 0x20, 0x6D, 0x00, 0x61,
                                     0x67, 0x6E, 0x61, 0x20, 0x61, 0x6C, 0x69, 0x10, 0x71, 0x75, 0x79, 0x00, 0x40, 0x65, 0x72, 0x61,
                                     0x74, 0x80, 0x80, 0x4F, 0x76, 0x6F, 0x6C, 0x75, 0x70, 0x74, 0x75, 0x00, 0x61, 0x2E, 0x20, 0x41,
                                     0x74, 0x20, 0x76, 0x65, 0x02, 0x72, 0x6F, 0x20, 0x65, 0x6F, 0x73, 0x10, 0x3D, 0x61, 0x08, 0x63,
                                     0x63, 0x75, 0x73, 0x10, 0x2F, 0x74, 0x20, 0x6A, 0x00, 0x75, 0x73, 0x74, 0x6F, 0x20, 0x64, 0x75,
                                     0x6F, 0xC0, 0x40, 0x52, 0x20, 0x1F, 0x65, 0x61, 0x20, 0x72, 0x65, 0x62, 0x02, 0x75, 0x6D, 0x2E,
                                     0x20, 0x53, 0x74, 0x00, 0x6C, 0x63, 0x80, 0x00, 0xA7, 0x61, 0x20, 0x6B, 0x61, 0x73, 0x64, 0x20,
                                     0x00, 0x67, 0x75, 0x62, 0x65, 0x72, 0x67, 0x72, 0x65, 0x30, 0x6E, 0x2C, 0x00, 0xAD, 0x00, 0xB9,
                                     0x61, 0x20, 0x74, 0x61, 0x08, 0x6B, 0x69, 0x6D, 0x61, 0x00, 0x1F, 0x73, 0x61, 0x6E, 0x11, 0x63,
                                     0x74, 0x75, 0x00, 0x5F, 0x73, 0x74, 0x20, 0xF1, 0x0B, 0xBF, 0x51, 0x0B, 0x2E, 0xF0, 0x1B, 0xF1,
                                     0x27, 0xF1, 0x27, 0xF1, 0x27, 0xF1, 0x27, 0xF1, 0x27, 0xFF, 0xF1, 0x27, 0xF1, 0x27, 0xF1, 0x27,
                                     0xF1, 0x27, 0xF1, 0x27, 0xF1, 0x27, 0xF1, 0x27, 0xF1, 0x27, 0xE0, 0xF1, 0x27, 0xF1, 0x0B, 0x51,
                                     0x27, 0x00, 0x00, 0x00};
// Random data
static const uint8_t v6[] = {0x39, 0xf5, 0x25, 0x57, 0x2d, 0xe0, 0x82, 0x54, 0xd3, 0xef, 0x33, 0x0f, 0x90, 0x78, 0x3e, 0xe1,
                             0xf8, 0xe4, 0x1f, 0x1f, 0xc8, 0x5c, 0xfc, 0xe6, 0x08, 0x3f, 0xfc, 0x99, 0xcc, 0x5b, 0xfc, 0xd6,
                             0x8d, 0x91, 0xe2, 0x5b, 0xa1, 0xa5, 0x4f, 0xd5, 0xb9, 0xbf, 0x5d, 0xe1, 0x07, 0x99, 0x12, 0x01,
                             0x33, 0xba, 0x31, 0x56, 0xf9, 0x1c, 0x27, 0xef, 0x58, 0xc7, 0xab, 0xa4, 0xdf, 0x4d, 0xa8, 0xdd,
                             0x4f, 0x18, 0x54, 0xa9, 0x7f, 0xee, 0x18, 0x0a, 0xa0, 0x95, 0x47, 0xab, 0x5b, 0xcf, 0x3d, 0x55,
                             0xa4, 0x70, 0x29, 0xbc, 0x63, 0x09, 0x6f, 0xfe, 0xf3, 0xf0, 0x8a, 0x22, 0x89, 0xdc, 0x6c, 0xf5,
                             0x86, 0x85, 0x58, 0xdd, 0x87, 0xa5, 0xdc, 0x14, 0x45, 0x11, 0x0d, 0x13, 0x2b, 0x50, 0x72, 0x58,
                             0xa4, 0x26, 0x41, 0x08, 0xa2, 0xb7, 0xd8, 0x08, 0x9f, 0xef, 0xdf, 0x53, 0x5d, 0xa0, 0xb8, 0xe3,
                             0xf4, 0x11, 0xc2, 0x09, 0xed, 0xd6, 0x68, 0x79, 0xe2, 0x91, 0x49, 0x9b, 0x2c, 0xfa, 0x4f, 0x23,
                             0xb7, 0x70, 0x90, 0xac, 0xdb, 0x53, 0x4f, 0x3e, 0xce, 0x18, 0x57, 0x0e, 0xfc, 0xf3, 0x44, 0xfe,
                             0x21, 0x8c, 0x65, 0x04, 0x47, 0x45, 0x85, 0xd9, 0x9c, 0xe3, 0x12, 0x0b, 0xa5, 0x29, 0xc5, 0xe7,
                             0x4f, 0x39, 0x5b, 0x08, 0x04, 0x4b, 0xa4, 0xdf, 0xb8, 0x8a, 0xd6, 0x29, 0x9a, 0x22, 0x6e, 0x12,
                             0x2d, 0x20, 0xee, 0x69, 0x30, 0xcb, 0x99, 0x7e, 0x0b, 0xbc, 0x38, 0x16, 0x4b, 0x51, 0x49, 0x09,
                             0xb5, 0x43, 0xdd, 0xb5, 0xda, 0x6b, 0xb5, 0xc0, 0xc3, 0xcf, 0xb2, 0xba, 0x57, 0xca, 0x3e, 0x33,
                             0xf6, 0x23, 0x4e, 0xeb, 0x95, 0x7b, 0x77, 0xbb, 0xca, 0x69, 0x0a, 0x8b, 0xa5, 0x7d, 0x70, 0xf4,
                             0x19, 0x81, 0x2b, 0xe3, 0x09, 0xa5, 0x5d, 0x4d, 0x94, 0x95, 0xe3, 0xf4, 0x73, 0x89, 0xac, 0x2a};
// Random repeating data
static const uint8_t v7[] = {0x94, 0xa0, 0xfc, 0x3a, 0x0c, 0x62, 0xdd, 0x8e, 0xe6, 0xe4, 0x7f, 0x0a, 0x7e, 0xce, 0x57, 0x88,
                             0x49, 0x8d, 0x0b, 0xe7, 0xc3, 0xfa, 0xcd, 0xcf, 0x9a, 0xf7, 0x65, 0x09, 0x8b, 0x08, 0xf1, 0x72,
                             0xa2, 0xcd, 0xab, 0xf2, 0xb1, 0xc9, 0x9b, 0x76, 0xd7, 0xb6, 0x3c, 0x15, 0x2b, 0xd3, 0x8e, 0x39,
                             0x94, 0xa0, 0xfc, 0x3a, 0x0c, 0x62, 0xdd, 0x8e, 0xe6, 0xe4, 0x7f, 0x0a, 0x7e, 0xce, 0x57, 0x88,
                             0x49, 0x8d, 0x0b, 0xe7, 0xc3, 0xfa, 0xcd, 0xcf, 0x9a, 0xf7, 0x65, 0x09, 0x8b, 0x08, 0xf1, 0x72,
                             0xa2, 0xcd, 0xab, 0xf2, 0xb1, 0xc9, 0x9b, 0x76, 0xd7, 0xb6, 0x3c, 0x15, 0x2b, 0xd3, 0x8e, 0x39,
                             0x09, 0x08, 0x01, 0xd9, 0x19, 0xad, 0xc0, 0x1b, 0xf8, 0xce, 0x90, 0xad, 0x49, 0xf4, 0xee, 0x0e,
                             0x09, 0x08, 0x01, 0xd9, 0x19, 0xad, 0xc0, 0x1b, 0xf8, 0xce, 0x90, 0xad, 0x49, 0xf4, 0xee, 0x0e};
// Bitmap data from img2h
static const uint32_t v8[] = {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x10000000, 0x00000000, 0x00000000,
                              0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00112011, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
                              0x00000000, 0x00000000, 0x00000000, 0x01110111, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
                              0x00000000, 0x11021120, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00002111,
                              0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00013110, 0x00000000, 0x00000000,
                              0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x11111111, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
                              0x00000000, 0x00000000, 0x00000000, 0x00000002, 0x40000000, 0x50000000, 0x50000000, 0x30000000, 0x30000000, 0x10000000,
                              0x10000000, 0x20000000, 0x22441234, 0x98443764, 0x98443764, 0x9a443744, 0x9a445744, 0xaa444c44, 0xa6444844, 0xa6464a44,
                              0x25442544, 0x76447a44, 0x76447a44, 0x76447844, 0x7644c844, 0x7644d744, 0x76449744, 0x76449764, 0x45214410, 0x45ca441b,
                              0x45ca441b, 0x45ca441b, 0x45ca441b, 0x45ca441b, 0x45ca441b, 0x45ca441b, 0x00234444, 0x0ba44444, 0x2c444444, 0xac444664,
                              0xa844a774, 0x97441974, 0x97445bc4, 0x97a44444, 0x02544441, 0xba444443, 0xc6444444, 0x74468a44, 0x7aa17a44, 0xccb2a444,
                              0x00144444, 0x2a444445, 0x44444443, 0x44444443, 0x44444443, 0x44444443, 0xcc644cb9, 0x97644b29, 0xb7644000, 0xb7644000,
                              0x00000021, 0x000000ca, 0x000000ca, 0x000000ca, 0x000000c7, 0x000000bb, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
                              0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x64564644, 0x546a4545, 0x446a4443, 0x44a84443,
                              0x44a84441, 0x44ac4441, 0x44a74441, 0x44b76442, 0x7644b764, 0x764427a4, 0x754407a4, 0x74450c84, 0x64450c84, 0x44430d74,
                              0x44410974, 0x44100b76, 0x45ca441b, 0x45ca441b, 0x45c8441b, 0x45c8443b, 0x45c84443, 0x45d75444, 0x45b7a444, 0x45277644,
                              0x27744444, 0x09644444, 0x2c444aa4, 0xa8446c74, 0xa8443974, 0x98443974, 0xd8441974, 0xd8441974, 0xc4444441, 0x84444a20,
                              0x84469200, 0x84410144, 0x8445a644, 0x74444444, 0x76444443, 0x7c644452, 0xb7644000, 0xb764400b, 0xb764400d, 0xb764400c,
                              0xb764400c, 0xb764400c, 0xb7644009, 0xb764400b, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
                              0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
                              0xc1b77d10, 0xb02bbb00, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xa1000b77, 0xb00002bb,
                              0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x910b778a, 0x2000bdcd, 0x00000000, 0x00000000,
                              0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xc7c1297c, 0xbbb002bb, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
                              0x00000000, 0x00000000, 0x9778aa20, 0x0bdc9000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
                              0xb7791000, 0x2bbb0000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
                              0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};

template <typename T>
auto toVector8(const T *in, std::size_t inSize) -> std::vector<uint8_t>
{
    std::vector<uint8_t> out(inSize);
    std::memcpy(out.data(), in, inSize);
    return out;
}

#define TO_VECTOR8(data) (toVector8((&data), sizeof(data)))

TEST_CASE("LZ10 roundtrip")
{
    // Run tests
    CATCH_REQUIRE_THROWS(encodeLZ10(TO_VECTOR8(v0)));
    CATCH_REQUIRE(TO_VECTOR8(v1) == decodeLZ10(encodeLZ10(TO_VECTOR8(v1))));
    CATCH_REQUIRE(TO_VECTOR8(v2) == decodeLZ10(encodeLZ10(TO_VECTOR8(v2))));
    CATCH_REQUIRE(TO_VECTOR8(v3) == decodeLZ10(encodeLZ10(TO_VECTOR8(v3))));
    CATCH_REQUIRE(TO_VECTOR8(v4) == decodeLZ10(encodeLZ10(TO_VECTOR8(v4))));
    CATCH_REQUIRE(TO_VECTOR8(v5) == decodeLZ10(encodeLZ10(TO_VECTOR8(v5))));
    CATCH_REQUIRE(TO_VECTOR8(v5) == decodeLZ10(TO_VECTOR8(v5_gbalzss)));
    // findDiff(v5_gbalzss, encodeLZ10(v5));
    // CATCH_REQUIRE(v5_gbalzss == encodeLZ10(v5));
    CATCH_REQUIRE(TO_VECTOR8(v6) == decodeLZ10(encodeLZ10(TO_VECTOR8(v6))));
    CATCH_REQUIRE(TO_VECTOR8(v7) == decodeLZ10(encodeLZ10(TO_VECTOR8(v7))));
    CATCH_REQUIRE(TO_VECTOR8(v8) == decodeLZ10(encodeLZ10(TO_VECTOR8(v8))));
}

TEST_CASE("LZ10 ratio")
{
    for (auto &testFile : TestFiles)
    {
        // open test file
        std::ifstream fs(DataPath + testFile.fileName, std::ios::binary | std::ios::in);
        REQUIRE(fs.is_open(), std::runtime_error, "Failed to open " << testFile.fileName << " for reading");
        // read all of the file data
        std::vector<uint8_t> fileData((std::istreambuf_iterator<char>(fs)), (std::istreambuf_iterator<char>()));
        // compress and decompress file data
        auto compressedData = encodeLZ10(fileData);
        std::cout << testFile.fileName << " compressed from " << fileData.size() << " to " << compressedData.size() << " bytes (" << static_cast<double>(compressedData.size()) / static_cast<double>(fileData.size()) * 100.0 << "%)" << std::endl;
        CATCH_REQUIRE(fileData == decodeLZ10(compressedData));
        CATCH_REQUIRE(compressedData.size() <= testFile.maxSize);
    }
}
