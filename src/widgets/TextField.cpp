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
        onClick([tf = this](Mouse::Button, const Point&){
            if (tf->getText().back() == '|')
                tf->setText(tf->getText().substr(0, tf->getText().size()-1));
            else
                tf->setText(tf->getText() + '|');
            
            if (!tf->added && tf->getParent())
            {
                tf->added = true;
                tf->getParent()->onClick([tf](Mouse::Button, const Point&){
                    if (tf->mouseLeft)
                    {
                        tf->setText(tf->getText().substr(0, tf->getText().size()-1));
                        tf->added = false;
                        tf->mouseLeft = false;
                        return true;
                    }
                    return false;
                });
            }
            
            return false;
        });
        onLeave([tf = this](){
            tf->mouseLeft = true;
            return false;
        });
        onKey([tf = this](Event::Type type, KeyCode keycode){
            if (type == Event::Type::KeyPressed && IsKeyCodeGraph(keycode))
            {
                if (tf->getText().back() == '|')
                {
                    std::string text = tf->getText();
                    text.back() = KeyCodeToGraph(keycode);
                    text += '|';
                    tf->setText(text);
                }
                else
                    tf->setText(tf->getText() + KeyCodeToGraph(keycode));
            }
                
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
