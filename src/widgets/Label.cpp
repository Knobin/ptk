//
//  widgets/Label.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-30.
//

// pTK Headers
#include "ptk/widgets/Label.hpp"

// Skia Headers
PTK_DISABLE_WARN_BEGIN()
#include "include/core/SkFont.h"
#include "include/core/SkTypeface.h"
PTK_DISABLE_WARN_END()

// C++ Headers
#include <cmath>

namespace pTK
{
    Label::Label()
        : Shape(), Text()
    {

    }
    
    void Label::onDraw(SkCanvas* canvas)
    {
        const Vec2f pos{static_cast<float>(getPosition().x), static_cast<float>(getPosition().y)};
        const Text::StrData data{ m_text.c_str(), m_text.size(), Text::Encoding::UTF8 };
        drawTextLine(canvas, data, getColor(), pos, getOutlineThickness(), getOutlineColor());
    }

    void Label::onTextUpdate()
    {
        const Size bounds = Vec2ToSize(getBoundsFromStr(m_text), std::ceilf);
        setMinSize(bounds);
        setSize(bounds);
        setMaxSize(bounds);
    }

    void Label::setText(const std::string& str)
    {
        m_text = str;
        onTextUpdate();
    }

    const std::string& Label::getText() const
    {
        return m_text;
    }
}
