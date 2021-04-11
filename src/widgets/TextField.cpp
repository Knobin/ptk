//
//  widgets/TextField.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-04-10.
//

// pTK Headers
#include "ptk/widgets/TextField.hpp"

namespace pTK
{
    TextField::TextField()
        : Rectangle(), Text()
    {
        onKey([tf = this](Event::Type type, KeyCode keycode){
            if (type == Event::Type::KeyPressed && IsKeyCodeGraph(keycode))
                tf->setText(tf->getText() + KeyCodeToGraph(keycode));
                
            return false;
        });
    }

    void TextField::onDraw(SkCanvas* canvas)
    {
        Rectangle::onDraw(canvas);
        
        const Size textBounds{getBounds()};
        const Size rectSize{getSize()};
        
        Point textPos{getPosition()};
        textPos.x += (rectSize.width - textBounds.width) / 2;
        textPos.y += (rectSize.height - textBounds.height) / 2;
        
        drawText(canvas, Color{0x000000FF}, textPos);
    }

    void TextField::onTextUpdate()
    {
        setMinSize(getBounds());
    }
}
