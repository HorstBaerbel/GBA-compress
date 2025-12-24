#include "memory.h"

#include <cstdint>

void LZ77UnCompWrite8bit2(const void *source, void *destination)
{
    uint32_t header = *reinterpret_cast<const uint32_t *>(source);
    const uint32_t uncompressedSize = (header >> 8);
    // skip header in source data
    auto srcPtr8 = reinterpret_cast<const uint8_t *>(source + 4);
    // decompress data
    uint32_t currentSize = 0;
    auto dstPtr8 = reinterpret_cast<uint8_t *>(destination);
    while (currentSize < uncompressedSize)
    {
        // read flags for next 8 tokens
        uint8_t flags = *srcPtr8++;
        for (int32_t flagBitIndex = 0; flagBitIndex < 8 && currentSize < uncompressedSize; ++flagBitIndex)
        {
            // check if next token is match or verbatim byte
            if ((flags & (0x80 >> flagBitIndex)) != 0)
            {
                // copy data for match from decoded buffer
                uint32_t matchLenght = (*srcPtr8 >> 4) + 3;
                uint32_t matchDistance = (*srcPtr8++ & 0xF) << 8;
                matchDistance |= *srcPtr8++;
                auto copyStartPtr = (dstPtr8 + currentSize) - (matchDistance + 1);
                // make sure to clamp copy size to not overrun buffer
                auto copyLength = matchLenght; // std::min(uncompressedSize - static_cast<uint32_t>(currentSize), matchLenght);
                memcpy(dstPtr8, copyStartPtr, copyLength);
                dstPtr8 += copyLength;
                currentSize += copyLength;
            }
            else
            {
                // store verbatim byte
                *dstPtr8++ = *srcPtr8++;
                currentSize++;
            }
        }
    }
}

void LZ77UnCompWrite16bit2(const void *source, void *destination)
{
    LZ77UnCompWrite8bit2(source, destination);
}