#pragma once

#include <cstdint>

// Functions for tile-base backgrounds in modes 0/1/2 and sprites.
namespace Tiles
{

    /// @brief s-tile 8x8@4bpp: 32bytes; 8 uints
    struct Tile16
    {
        uint32_t data[8];
    };

    /// @brief d-tile: 8x8@8bpp: 64bytes; 16 uints
    struct Tile256
    {
        uint32_t data[16];
    };

    /// @brief Start of tile memory (== start of VRAM)
    constexpr uint32_t TileMem = 0x06000000;

    /// @brief Tile memory interpreted as 16 color tiles
    auto const TileMem16{reinterpret_cast<Tile16 *>(TileMem)};

    /// @brief Tile memory interpreted as 256 color tiles
    auto const TileMem256{reinterpret_cast<Tile256 *>(TileMem)};

    /// @brief This is where the tiles aka the bitmap data for the tiles starts.
    enum class TileBase : uint16_t
    {
        Base0000 = (0 << 2),
        Base4000 = (1 << 2),
        Base8000 = (2 << 2),
        BaseC000 = (3 << 2)
    };

    /// @brief Convert tile base to memory address
    template <typename T>
    constexpr T *TILE_BASE_TO_MEM(TileBase b)
    {
        return reinterpret_cast<T *>(TileMem + ((uint32_t(b)) << 12));
    }

    /// @brief This is where the screen aka the map data for the tiles starts.
    enum class ScreenBase : uint16_t
    {
        Base0000 = (0 << 8),
        Base0800 = (1 << 8),
        Base1000 = (2 << 8),
        Base1800 = (3 << 8),
        Base2000 = (4 << 8),
        Base2800 = (5 << 8),
        Base3000 = (6 << 8),
        Base3800 = (7 << 8),
        Base4000 = (8 << 8),
        Base4800 = (9 << 8),
        Base5000 = (10 << 8),
        Base5800 = (11 << 8),
        Base6000 = (12 << 8),
        Base6800 = (13 << 8),
        Base7000 = (14 << 8),
        Base7800 = (15 << 8),
        Base8000 = (16 << 8),
        Base8800 = (17 << 8),
        Base9000 = (18 << 8),
        Base9800 = (19 << 8),
        BaseA000 = (20 << 8),
        BaseA800 = (21 << 8),
        BaseB000 = (22 << 8),
        BaseB800 = (23 << 8),
        BaseC000 = (24 << 8),
        BaseC800 = (25 << 8),
        BaseD000 = (26 << 8),
        BaseD800 = (27 << 8),
        BaseE000 = (28 << 8),
        BaseE800 = (29 << 8),
        BaseF000 = (30 << 8),
        BaseF800 = (31 << 8)
    };

    /// @brief Convert screen base to memory address
    template <typename T>
    constexpr T *SCREEN_BASE_TO_MEM(ScreenBase b)
    {
        return reinterpret_cast<T *>(TileMem + ((uint32_t(b)) << 3));
    }

    /// @brief Screen map value to flip tile horizontally
    constexpr uint16_t FlipHorizontal = 1 << 10;

    /// @brief Screen map value to flip tile vertically
    constexpr uint16_t FlipVertical = 1 << 11;
}
