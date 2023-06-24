//
//  util/Texture.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-06-23.
//

// pTK Headers
#include "ptk/util/Texture.hpp"

// Skia Headers
PTK_DISABLE_WARN_BEGIN()
#include "include/core/SkData.h"
#include "include/core/SkImage.h"
#include "include/core/SkPixmap.h"
PTK_DISABLE_WARN_END()

namespace pTK
{
    ///////////////////////////////////////////////////////////////////////////////////////////////////

    [[nodiscard]] static constexpr SkColorType GetSkColorType(ColorType colorType) noexcept
    {
        struct LookupItem
        {
            ColorType type{};
            SkColorType skType{};
        };
        constexpr std::array<LookupItem, 4> lookup{
            LookupItem{ColorType::Unknown, SkColorType::kUnknown_SkColorType},
            LookupItem{ColorType::RGBA_8888, SkColorType::kRGBA_8888_SkColorType},
            LookupItem{ColorType::RGB_888x, SkColorType::kRGB_888x_SkColorType},
            LookupItem{ColorType::BGRA_8888, SkColorType::kBGRA_8888_SkColorType}};

        constexpr auto size{static_cast<std::ptrdiff_t>(lookup.size())};
        for (std::ptrdiff_t i{1}; i < size; ++i)
            if (colorType == lookup[i].type)
                return lookup[i].skType;

        return lookup[0].skType;
    }

    [[nodiscard]] static constexpr bool IsColorTypeOpaque(ColorType colorType) noexcept
    {
        return GetColorTypeInfo(colorType).channelBits.a == 255;
    }

    [[nodiscard]] static constexpr SkAlphaType GetSkAlphaType(ColorType colorType) noexcept
    {
        if (colorType == ColorType::Unknown)
            return SkAlphaType::kUnknown_SkAlphaType;
        if (IsColorTypeOpaque(colorType))
            return SkAlphaType::kOpaque_SkAlphaType;
        return SkAlphaType::kUnpremul_SkAlphaType;
    }

    [[nodiscard]] static SkPixmap GetSkPixmap(const Pixmap& pixmap) noexcept
    {
        const auto width{static_cast<int>(pixmap.width())};
        const auto height{static_cast<int>(pixmap.height())};

        const auto ct{pixmap.colorType()};
        const auto info{SkImageInfo::Make(width, height, GetSkColorType(ct), GetSkAlphaType(ct))};
        const auto rowBytes{static_cast<std::size_t>(pixmap.width() * (GetColorTypeInfo(ct).totalBits / 8))};
        return SkPixmap{info, pixmap.data(), rowBytes};
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////

    Texture::Texture(const ImmutableBuffer& buffer)
    {
        sk_sp<SkData> data{SkData::MakeWithoutCopy(buffer.data(), buffer.size())};
        if (data)
        {
            m_image = std::make_unique<sk_sp<SkImage>>(SkImage::MakeFromEncoded(data));
            if (m_image && m_image->get())
            {
                m_size.width = static_cast<Size::value_type>(m_image->get()->width());
                m_size.height = static_cast<Size::value_type>(m_image->get()->height());
            }
        }
    }

    Texture::Texture(const Pixmap& pixmap)
    {
        if (pixmap.isValid())
        {
            SkPixmap data{GetSkPixmap(pixmap)};
            m_image = std::make_unique<sk_sp<SkImage>>(SkImage::MakeRasterCopy(data));
            if (m_image && m_image->get())
            {
                m_size.width = static_cast<Size::value_type>(m_image->get()->width());
                m_size.height = static_cast<Size::value_type>(m_image->get()->height());
            }
        }
    }

    // This destructor must be here (and not defaulted) to be able to properly delete SkImage.
    // Otherwise, the user must include the SkImage header for its destructor to be available.
    Texture::~Texture() {}

    ///////////////////////////////////////////////////////////////////////////////////////////////////

    bool Texture::isValid() const
    {
        return ((m_image && m_image->get()) ? m_image->get()->isValid(nullptr) : false);
    }

    SkImage *Texture::skImage() const noexcept
    {
        return (m_image) ? m_image->get() : nullptr;
    }
}