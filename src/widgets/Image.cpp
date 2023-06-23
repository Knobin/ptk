//
//  widgets/Image.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-11-25.
//

// Local Headers
#include "../include/Log.hpp"

// pTK Headers
#include "ptk/widgets/Image.hpp"

// Skia Headers
PTK_DISABLE_WARN_BEGIN()
#include "include/core/SkCanvas.h"
#include "include/core/SkData.h"
#include "include/core/SkImage.h"
#include "include/core/SkPixmap.h"
PTK_DISABLE_WARN_END()

namespace pTK
{
    Image::Image()
        : Widget(),
          m_image{nullptr},
          m_scale{1.0f, 1.0f}
    {}

    Image::Image(const ImmutableBuffer& buffer)
        : Widget(),
          m_image{nullptr},
          m_scale{1.0f, 1.0f}
    {
        sk_sp<SkData> data{SkData::MakeWithoutCopy(buffer.data(), buffer.size())};
        if (data)
        {
            m_image = std::make_unique<sk_sp<SkImage>>(SkImage::MakeFromEncoded(data));
            if (m_image && m_image->get())
            {
                const float w = static_cast<float>(m_image->get()->width()) * m_scale.x;
                const float h = static_cast<float>(m_image->get()->height()) * m_scale.y;
                setSize(Size(static_cast<Size::value_type>(w), static_cast<Size::value_type>(h)));
            }
        }
    }

    [[nodiscard]] static constexpr SkColorType GetSkColorType(ColorType colorType) noexcept
    {
        struct LookupItem
        {
            ColorType type{};
            SkColorType skType{};
        };
        constexpr std::array<LookupItem, 4> lookup{
            LookupItem{ColorType::Unknown, SkColorType::kUnknown_SkColorType},
            LookupItem{ColorType::RBGA_8888, SkColorType::kRGBA_8888_SkColorType},
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

    Image::Image(const Pixmap& pixmap)
        : Widget(),
          m_image{nullptr},
          m_scale{1.0f, 1.0f}
    {
        if (pixmap.isValid())
        {
            SkPixmap data{GetSkPixmap(pixmap)};
            m_image = std::make_unique<sk_sp<SkImage>>(SkImage::MakeRasterCopy(data));
            if (m_image && m_image->get())
            {
                const float w = static_cast<float>(m_image->get()->width()) * m_scale.x;
                const float h = static_cast<float>(m_image->get()->height()) * m_scale.y;
                setSize(Size(static_cast<Size::value_type>(w), static_cast<Size::value_type>(h)));
            }
        }
    }

    bool Image::isValid() const
    {
        return ((m_image && m_image->get()) ? m_image->get()->isValid(nullptr) : false);
    }

    void Image::onDraw(Canvas* canvas)
    {
        if (m_image && m_image->get())
            canvas->drawImage(getPosition(), getSize(), m_image->get());
    }

    const Vec2f& Image::getScale() const
    {
        return m_scale;
    }

    void Image::setScale(float x, float y)
    {
        applyScale(x, y);
    }

    void Image::setScale(const Vec2f& scale)
    {
        applyScale(scale.x, scale.y);
    }

    void Image::applyScale(float x, float y)
    {
        if (x > 0.0f)
            m_scale.x = x;
        if (y > 0.0f)
            m_scale.y = y;

        if (m_image && m_image->get())
        {
            const float w = static_cast<float>(m_image->get()->width()) * m_scale.x;
            const float h = static_cast<float>(m_image->get()->height()) * m_scale.y;
            setSize(Size{static_cast<Size::value_type>(w), static_cast<Size::value_type>(h)});
        }
    }
} // namespace pTK
