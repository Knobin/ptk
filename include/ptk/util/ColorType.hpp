//
//  util/ColorType.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-06-22.
//

// TODO(knobin): Add documentation.

#ifndef PTK_UTIL_COLOR_TYPE_HPP
#define PTK_UTIL_COLOR_TYPE_HPP

// C++ Headers
#include <array>
#include <cstddef>
#include <cstdint>

namespace pTK
{
    enum class ColorType
    {
        Unknown,
        RBGA_8888, // 8 bits per channel totaling 32 bits per pixel.
        RGB_888x,  // 8 bits per channel with blank alpha totaling 32 bits per pixel.
        BGRA_8888  // 8 bits per channel totaling 32 bits per pixel.
    };

    struct ColorTypeInfo
    {
        struct ChannelBits
        {
            uint8_t a{};
            uint8_t r{};
            uint8_t g{};
            uint8_t b{};
        } channelBits{};
        std::size_t totalBits{};
    };

    [[nodiscard]] constexpr ColorTypeInfo GetColorTypeInfo(ColorType colorType) noexcept
    {
        struct LookupItem {ColorType type{}; ColorTypeInfo info{};};
        constexpr std::array<LookupItem, 4> lookup{
            LookupItem{ColorType::Unknown, ColorTypeInfo{}},
            LookupItem{ColorType::RBGA_8888, ColorTypeInfo{{8, 8, 8, 8}, 8 * 4}},
            LookupItem{ColorType::RGB_888x, ColorTypeInfo{{0, 8, 8, 8}, 8 * 4}},
            LookupItem{ColorType::BGRA_8888, ColorTypeInfo{{8, 8, 8, 8}, 8 * 4}}
        };

        constexpr auto size{static_cast<std::ptrdiff_t>(lookup.size())};
        for (std::ptrdiff_t i{1}; i < size; ++i)
            if (colorType == lookup[i].type)
                return lookup[i].info;

        return lookup[0].info;
    }
} // namespace pTK

#endif // PTK_UTIL_COLOR_TYPE_HPP
