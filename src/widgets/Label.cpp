//
//  widgets/Label.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-30.
//

// Local Headers
#include "ptk/widgets/Label.hpp"

// Skia Headers
#include "include/core/SkFont.h"
#include "include/core/SkTypeface.h"

// C++ Headers
#include <cmath>

namespace pTK
{
    Label::Label()
        : Shape(), Font(), m_text{""}
    {
    }
    
    void Label::setFontFamily(const std::string& fontFamily)
    {
        Font::setFontFamily(fontFamily);
        calculateBounds();
    }
    
    void Label::setFontSize(uint fontSize)
    {
        Font::setFontSize(fontSize);
        calculateBounds();
    }
    
    void Label::setText(const std::string& text)
    {
        m_text = text;
        calculateBounds();
    }
    
    const std::string& Label::getText() const
    {
        return m_text;
    }
    
    void Label::calculateBounds()
    {
        std::string fontFamily = getFontFamily();
        uint fontSize = getFontSize();
        
        // Font
        SkFont font;
        if (fontFamily == "")
            font = SkFont(SkTypeface::MakeDefault(), fontSize);
        else
         font = SkFont(SkTypeface::MakeFromName(fontFamily.c_str(), SkFontStyle::Normal()), fontSize);
        
        SkRect bounds;
        font.measureText(m_text.c_str(), m_text.size(), SkTextEncoding::kUTF8, &bounds);
        setSize(Size((int)std::ceil(bounds.width()), (int)std::ceil(bounds.height()))); // This will call redraw.
    }
    
    void Label::onDraw(SkCanvas* canvas)
    {
        // Set Color.
        SkPaint paint;
        paint.setAntiAlias(true);
        Color color = getColor();
        paint.setARGB(color.a, color.r, color.g, color.b);
        
        SkFont font;
        std::string fontFamily = getFontFamily();
        uint fontSize = getFontSize();
        if (fontFamily == "")
            font = SkFont(SkTypeface::MakeDefault(), fontSize);
        else
            font = SkFont(SkTypeface::MakeFromName(fontFamily.c_str(), SkFontStyle::Normal()), fontSize);
        
        // Calculate Bounds and Position.
        SkRect bounds;
        font.measureText(m_text.c_str(), m_text.size(), SkTextEncoding::kUTF8, &bounds);
        SkPoint pos{convertToSkPoint(getPosition())};
        
        // Outline
        float outlineThickness = getOutlineThickness();
        paint.setStrokeWidth(outlineThickness);
        if (outlineThickness > 0.0f)
        {
            paint.setStyle(SkPaint::kFill_Style);
        }else
        {
            paint.setStyle(SkPaint::kStrokeAndFill_Style);
        }
        
        canvas->drawString(m_text.c_str(), pos.fX + (-1*bounds.x()), pos.fY + (-1*bounds.y()), font, paint);
        
        if (outlineThickness > 0.0f)
        {
            // Draw Outline
            Color outColor = getOutlineColor();
            paint.setARGB(outColor.a, outColor.r, outColor.g, outColor.b);
            paint.setStyle(SkPaint::kStroke_Style);
            canvas->drawString(m_text.c_str(), pos.fX + (-1*bounds.x()), pos.fY + (-1*bounds.y()), font, paint);
        }
    }
}
