//
//  core/Canvas.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-03-02.
//

// pTK Headers
#include "ptk/core/Canvas.hpp"

// Skia Headers
PTK_DISABLE_WARN_BEGIN()
#include "include/core/SkCanvas.h"
#include "include/core/SkFont.h"
#include "include/core/SkFontMetrics.h"
#include "include/core/SkTypeface.h"
PTK_DISABLE_WARN_END()

namespace pTK
{
    static SkPoint ToSkPoint(const Point& pos, const Vec2f& scale = {1.0f, 1.0f})
    {
        return SkPoint{static_cast<float>(pos.x) * scale.x, static_cast<float>(pos.y) * scale.y};
    }

    static SkPoint ToSkPoint(const Size& size, const Vec2f& scale = {1.0f, 1.0f})
    {
        return SkPoint{static_cast<float>(size.width) * scale.x, static_cast<float>(size.height) * scale.y};
    }

    static SkPaint ToSkPaint(const Color& color)
    {
        SkPaint paint{};
        paint.setAntiAlias(true);
        paint.setARGB(color.a, color.r, color.g, color.b);
        return paint;
    }

    ///////////////////////////////////////////////////////////////////////////////

    static SkTextEncoding EncodingToSkTextEncoding(Text::Encoding encoding)
    {
        if (encoding == Text::Encoding::UTF16)
            return SkTextEncoding::kUTF16;
        if (encoding == Text::Encoding::UTF32)
            return SkTextEncoding::kUTF32;

        return SkTextEncoding::kUTF8;
    }

    static std::size_t SpaceCount(const Text::StrData& data)
    {
        std::size_t count{0};

        const void* ptr = data.text;

        for (std::size_t i{0}; i < data.size; ++i)
        {
            uint32_t ch{0};

            if (data.encoding == Text::Encoding::UTF8)
            {
                const auto* c = static_cast<const uint8_t*>(ptr);
                ch = static_cast<uint32_t>(c[i]);
            }

            if (data.encoding == Text::Encoding::UTF16)
            {
                const auto* c = static_cast<const uint16_t*>(ptr);
                ch = static_cast<uint32_t>(c[i]);
            }

            if (data.encoding == Text::Encoding::UTF32)
            {
                const auto* c = static_cast<const uint32_t*>(ptr);
                ch = static_cast<uint32_t>(c[i]);
            }

            if (ch == 0x20)
                ++count;
            else
                break;
        }

        return count;
    }

    static float StartSpaceOffset(const SkFont& font, const Text::StrData& data)
    {
        // This function only handles " " for now.
        // TODO: Support for no-break space should be added.

        float offset{0.0f};
        std::size_t spaceCount{SpaceCount(data)};

        if (spaceCount > 0)
        {
            SkRect tmp{};
            offset = font.measureText(data.text, spaceCount, EncodingToSkTextEncoding(data.encoding), &tmp);
        }

        return offset;
    }

    static constexpr std::size_t EncodingByteSize(Text::Encoding encoding)
    {
        if (encoding == Text::Encoding::UTF16)
            return 2;
        if (encoding == Text::Encoding::UTF32)
            return 4;

        return 1;
    }

    static float CapHeight(const SkFont* font)
    {
        SkFontMetrics metrics{};
        font->getMetrics(&metrics);
        return std::abs(metrics.fCapHeight);
    }

    ///////////////////////////////////////////////////////////////////////////////

    void Canvas::drawRect(Point pos, Size size, Color color) const
    {
        drawRoundRect(pos, size, color, 0.0f);
    }

    void Canvas::drawRect(Point pos, Size size, Color color, Color outlineColor, float outlineThickness) const
    {
        drawRoundRect(pos, size, color, 0.0f, outlineColor, outlineThickness);
    }

    void Canvas::drawRoundRect(Point pos, Size size, Color color, float cornerRadius) const
    {
        SkPoint skPos{ToSkPoint(pos)};
        SkPoint skSize{ToSkPoint(size)};
        skSize += skPos; // skia needs the size to be pos+size.

        SkPaint paint{ToSkPaint(color)};
        paint.setStrokeWidth(0.0f);
        paint.setStyle(SkPaint::kStrokeAndFill_Style);

        SkRect rect{};
        rect.set(skPos, skSize);
        skCanvas->drawRoundRect(rect, cornerRadius, cornerRadius, paint);
    }

    void Canvas::drawRoundRect(Point pos, Size size, Color color, float cornerRadius, Color outlineColor,
                               float outlineThickness) const
    {
        SkPoint skPos{ToSkPoint(pos)};
        SkPoint skSize{ToSkPoint(size)};
        skSize += skPos; // skia needs the size to be pos+size.

        // Outline.
        const float halfOutlineThickness{outlineThickness / 2.0f};
        skPos.fX += halfOutlineThickness;
        skPos.fY += halfOutlineThickness;
        skSize.fX -= halfOutlineThickness;
        skSize.fY -= halfOutlineThickness;

        // Set Color.
        SkPaint paint{ToSkPaint(color)};
        paint.setStrokeWidth(outlineThickness);
        if (outlineThickness > 0.0f)
            paint.setStyle(SkPaint::kFill_Style);
        else
            paint.setStyle(SkPaint::kStrokeAndFill_Style);

        // Draw Rect.
        SkRect rect{};
        rect.set(skPos, skSize);
        skCanvas->drawRoundRect(rect, cornerRadius, cornerRadius, paint);

        if (outlineThickness > 0.0f)
        {
            // Draw Outline.
            paint.setARGB(outlineColor.a, outlineColor.r, outlineColor.g, outlineColor.b);
            paint.setStyle(SkPaint::kStroke_Style);
            skCanvas->drawRoundRect(rect, cornerRadius, cornerRadius, paint);
        }
    }

    float Canvas::drawTextLine(const Text::StrData& data, const Color& color, const Vec2f& pos,
                               const SkFont* font) const
    {
        SkPaint paint{ToSkPaint(color)};
        paint.setStyle(SkPaint::kStrokeAndFill_Style);

        return drawTextLineWithPaint(data, pos, font, &paint);
    }

    float Canvas::drawTextLine(const Text::StrData& data, const Color& color, const Vec2f& pos, const SkFont* font,
                               float outlineSize, const Color& outColor) const
    {
        if (!(outlineSize > 0.0f))
            return drawTextLine(data, color, pos, font);

        SkPaint paint{ToSkPaint(color)};
        paint.setStrokeWidth(outlineSize);
        paint.setStyle(SkPaint::kFill_Style);
        float advance = drawTextLineWithPaint(data, pos, font, &paint);

        paint.setARGB(outColor.a, outColor.r, outColor.g, outColor.b);
        paint.setStyle(SkPaint::kStroke_Style);
        drawTextLineWithPaint(data, pos, font, &paint);

        return advance;
    }

    float Canvas::drawTextLineWithPaint(const Text::StrData& data, const Vec2f& pos, const SkFont* font,
                                        SkPaint* paint) const
    {
        SkRect bounds{};

        std::size_t byteLength = data.size * EncodingByteSize(data.encoding);
        float advance = font->measureText(data.text, byteLength, EncodingToSkTextEncoding(data.encoding), &bounds);

#ifdef PTK_DRAW_TEXT_RECT
        Point p{static_cast<int>(pos.x), static_cast<int>(pos.y)};
        Size s{static_cast<Size::value_type>(std::ceil(advance)),
               static_cast<Size::value_type>(std::ceil(CapHeight(font)))};
        drawRect(p, s, Color{0xFF1212AA});
#endif

        SkScalar x = pos.x + StartSpaceOffset(*font, data) + (-1 * bounds.x());
        SkScalar y = pos.y + CapHeight(font);

        skCanvas->drawSimpleText(data.text, byteLength, EncodingToSkTextEncoding(data.encoding), x, y, *font, *paint);

        return advance;
    }

    void Canvas::drawImage(Point pos, Size size, const SkImage* image) const
    {
        SkPoint skPos{ToSkPoint(pos)};
        SkPoint skSize{ToSkPoint(size)};
        skSize += skPos; // skia needs the size to be pos+size.

        SkRect dst{};
        dst.set(skPos, skSize);
        skCanvas->drawImageRect(image, dst, SkSamplingOptions(), nullptr);
    }

} // namespace pTK
