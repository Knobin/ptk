//
//  util/ColorType.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-06-22.
//

#ifndef PTK_UTIL_COLOR_TYPE_HPP
#define PTK_UTIL_COLOR_TYPE_HPP

// C++ Headers
#include <array>
#include <cstddef>
#include <cstdint>

namespace pTK
{
    /** ColorType enum class implementation.

        Defines how pixels are stored and used, how many channels and channel
        sizes the pixel have.
    */
    enum class ColorType
    {
        Unknown,   // Uninitialized.
        RGBA_8888, // 8 bits per channel totaling 32 bits per pixel.
        RGB_888x,  // 8 bits per channel with blank alpha totaling 32 bits per pixel.
        BGRA_8888  // 8 bits per channel totaling 32 bits per pixel.
    };

    /** ColorTypeInfo struct implementation.

        Represents the ColorType in a better format with channel information on what position
        they have in the pixel, how many bits they individually use how many bits in total in
        the pixels.

        Note: Channels not used by the pixel has a size of 0 and not used has channel index of 0.

        Use the size of the channel to determine if the channel is used or not.
    */
    struct ColorTypeInfo
    {
        struct ChannelIndex
        {
            std::ptrdiff_t a{};
            std::ptrdiff_t r{};
            std::ptrdiff_t g{};
            std::ptrdiff_t b{};
        } channelIndex{};
        struct ChannelBits
        {
            uint8_t a{};
            uint8_t r{};
            uint8_t g{};
            uint8_t b{};
        } channelBits{};
        std::size_t totalBits{};
    };

    /** Function for retrieving ColorTypeInfo from ColorType.

        @return     color type information
    */
    [[nodiscard]] constexpr ColorTypeInfo GetColorTypeInfo(ColorType colorType) noexcept
    {
        struct LookupItem {ColorType type{}; ColorTypeInfo info{};};
        constexpr std::array<LookupItem, 4> lookup{
            LookupItem{ColorType::Unknown, ColorTypeInfo{}},
            LookupItem{ColorType::RGBA_8888, ColorTypeInfo{{3, 0, 1, 2}, {8, 8, 8, 8}, 8 * 4}},
            LookupItem{ColorType::RGB_888x, ColorTypeInfo{{3, 0, 1, 2}, {0, 8, 8, 8}, 8 * 4}},
            LookupItem{ColorType::BGRA_8888, ColorTypeInfo{{3, 2, 1, 0}, {8, 8, 8, 8}, 8 * 4}}
        };

        constexpr auto size{static_cast<std::ptrdiff_t>(lookup.size())};
        for (std::size_t i{1}; i < size; ++i)
            if (colorType == lookup[i].type)
                return lookup[i].info;

        return lookup[0].info;
    }
} // namespace pTK

#endif // PTK_UTIL_COLOR_TYPE_HPP
