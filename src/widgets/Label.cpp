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
        drawText(canvas, getText(), getColor(), pos, getOutlineThickness(), getOutlineColor());
    }

    void Label::onTextUpdate()
    {
        const Size bounds{getBounds()};
        setMinSize(bounds);
        setSize(bounds);
        setMaxSize(bounds);
    }
}
