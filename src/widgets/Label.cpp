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
        : Shape(), Text()
    {
    }
    
    bool Label::setFontFromFile(const std::string& path)
    {
        bool status = Text::setFontFromFile(path);
        if (status)
            setSize(getBounds());

        return status;
    }

    bool Label::setFontFamily(const std::string& fontFamily)
    {
        bool status = Text::setFontFamily(fontFamily);
        if (status)
            setSize(getBounds());

        return status;
    }
    
    void Label::setFontSize(uint fontSize)
    {
        Text::setFontSize(fontSize);
        setSize(getBounds());
    }
    
    void Label::setText(const std::string& text)
    {
        Text::setText(text);
        setSize(getBounds());
    }
    
    void Label::onDraw(SkCanvas* canvas)
    {
        // Set Color.
        SkPaint paint;
        paint.setAntiAlias(true);
        Color color = getColor();
        paint.setARGB(color.a, color.r, color.g, color.b);
        
        // Calculate Bounds and Position.
        SkRect bounds;
        std::string text = getText();
        getFont()->skFont()->measureText(text.c_str(), text.size(), SkTextEncoding::kUTF8, &bounds);
        SkPoint pos{convertToSkPoint(getPosition())};
        
        // Outline
        float outlineThickness = getOutlineThickness();
        paint.setStrokeWidth(outlineThickness);
        if (outlineThickness > 0.0f)
                paint.setStyle(SkPaint::kFill_Style);
        else
            paint.setStyle(SkPaint::kStrokeAndFill_Style);
        
        canvas->drawString(text.c_str(), pos.fX + (-1*bounds.x()), pos.fY + (-1*bounds.y()), *getFont()->skFont(), paint);
        
        if (outlineThickness > 0.0f)
        {
            // Draw Outline
            Color outColor = getOutlineColor();
            paint.setARGB(outColor.a, outColor.r, outColor.g, outColor.b);
            paint.setStyle(SkPaint::kStroke_Style);
            canvas->drawString(text.c_str(), pos.fX + (-1*bounds.x()), pos.fY + (-1*bounds.y()), *getFont()->skFont(), paint);
        }
    }
}
