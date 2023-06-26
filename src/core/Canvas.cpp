//
//  core/Canvas.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-03-02.
//

// pTK Headers
#include "ptk/core/Canvas.hpp"
#include "ptk/util/Vec2.hpp"

// Skia Headers
PTK_DISABLE_WARN_BEGIN()
#include "include/core/SkCanvas.h"
#include "include/core/SkFont.h"
#include "include/core/SkFontMetrics.h"
#include "include/core/SkTypeface.h"
PTK_DISABLE_WARN_END()

namespace pTK
{
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
        const Vec2f fPos{static_cast<float>(pos.x), static_cast<float>(pos.y)};
        const Vec2f fSize{static_cast<float>(size.width), static_cast<float>(size.height)};
        drawRoundRect(fPos, fSize, color, 0.0f);
    }

    void Canvas::drawRect(Point pos, Size size, Color color, Color outlineColor, float outlineThickness) const
    {
        const Vec2f fPos{static_cast<float>(pos.x), static_cast<float>(pos.y)};
        const Vec2f fSize{static_cast<float>(size.width), static_cast<float>(size.height)};
        drawRoundRect(fPos, fSize, color, 0.0f, outlineColor, outlineThickness);
    }

    void Canvas::drawRect(Vec2f pos, Vec2f size, Color color, Color outlineColor, float outlineThickness) const
    {
        drawRoundRect(pos, size, color, 0.0f, outlineColor, outlineThickness);
    }

    void Canvas::drawRoundRect(Point pos, Size size, Color color, float cornerRadius) const
    {
        const Vec2f fPos{static_cast<float>(pos.x), static_cast<float>(pos.y)};
        const Vec2f fSize{static_cast<float>(size.width), static_cast<float>(size.height)};
        drawRoundRect(fPos, fSize, color, cornerRadius);
    }

    void Canvas::drawRoundRect(Vec2f pos, Vec2f size, Color color, float cornerRadius) const
    {
        SkPoint skPos{pos.x, pos.y};
        SkPoint skSize{size.x, size.y};
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
        const Vec2f fPos{static_cast<float>(pos.x), static_cast<float>(pos.y)};
        const Vec2f fSize{static_cast<float>(size.width), static_cast<float>(size.height)};
        drawRoundRect(fPos, fSize, color, cornerRadius, outlineColor, outlineThickness);
    }

    void Canvas::drawRoundRect(Vec2f pos, Vec2f size, Color color, float cornerRadius, Color outlineColor,
                               float outlineThickness) const
    {
        SkPoint skPos{pos.x, pos.y};
        SkPoint skSize{size.x, size.y};
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

    ///////////////////////////////////////////////////////////////////////////////

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
        const Vec2f fPos{static_cast<float>(pos.x), static_cast<float>(pos.y)};
        const Vec2f fSize{static_cast<float>(size.width), static_cast<float>(size.height)};
        drawImage(fPos, fSize, image);
    }

    void Canvas::drawImage(Vec2f pos, Vec2f size, const SkImage* image) const
    {
        SkPoint skPos{pos.x, pos.y};
        SkPoint skSize{size.x, size.y};
        skSize += skPos; // skia needs the size to be pos+size.

        SkRect dst{};
        dst.set(skPos, skSize);
        skCanvas->drawImageRect(image, dst, SkSamplingOptions(), nullptr);
    }

    ///////////////////////////////////////////////////////////////////////////////

    void Canvas::save() const
    {
        skCanvas->save();
    }

    void Canvas::saveLayer(SkPaint* paint) const
    {
        skCanvas->saveLayer(nullptr, paint);
    }

    void Canvas::restore() const
    {
        skCanvas->restore();
    }

    ///////////////////////////////////////////////////////////////////////////////

    void Canvas::translate(float x, float y) const
    {
        skCanvas->translate(x, y);
    }

    void Canvas::scale(float x, float y) const
    {
        skCanvas->scale(x, y);
    }

    void Canvas::rotate(float degrees) const
    {
        skCanvas->rotate(degrees);
    }

    void Canvas::rotate(float degrees, float x, float y) const
    {
        skCanvas->rotate(degrees, x, y);
    }

    void Canvas::transform(float a, float b, float c, float d, float e, float f) const
    {
        const SkMatrix transform{SkMatrix::MakeAll(a, b, c, d, e, f, 0, 0, 1)};
        skCanvas->concat(transform);
    }

} // namespace pTK
