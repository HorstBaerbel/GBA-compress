#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace Compression
{
    constexpr uint8_t TYPE_MARKER = 0x40;          // Used to detect LZ4 compression in data
    constexpr uint32_t MIN_MATCH_LENGTH = 4;       // A match needs at least 3 bytes to encode, thus 4 is the minimum match length
    constexpr uint32_t MAX_MATCH_LENGTH = 783;     // We want max. 4+3*8 bits to encode match length -> 783 = 15+3*255 + (MIN_MATCH_LENGTH - 1)
    constexpr uint32_t MAX_MATCH_DISTANCE = 65535; // We have max. 16 bits to encode match distance
    constexpr uint8_t LITERAL_LENGTH_SHIFT = 4;    // Left-shift of literal length in token byte
    constexpr uint8_t LENGTH_MASK = 0x0F;          // Used for masking literal and match lengths

    /// @brief Compress input data using LZ4 variant 40h
    /// @note This is probably not 100% stream compatible with regular LZ4
    auto encodeLZ4_40(const std::vector<uint8_t> &data, bool vramCompatible = false) -> std::vector<uint8_t>;

    /// @brief Decompress input data using LZ4 variant 40h
    auto decodeLZ4_40(const std::vector<uint8_t> &data, bool vramCompatible = false) -> std::vector<uint8_t>;
}
