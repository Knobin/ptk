//
//  widgets/TextField.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-04-10.
//

#ifndef PTK_WIDGETS_TEXTFIELD_HPP
#define PTK_WIDGETS_TEXTFIELD_HPP

// pTK Headers
#include "ptk/widgets/Rectangle.hpp"
#include "ptk/core/Text.hpp"
#include "ptk/util/Vec2.hpp"

namespace pTK
{
    /** TextField class implementation.
     
    */
    class TextField : public Rectangle, public Text
    {
    public:
        /** Constructs Label with default values.
         
            @return    default initialized Label
        */
        TextField();
        virtual ~TextField() = default;
        
        /** Draw function.
            Function is called when it is time to draw.
         
            Derived from Drawable.
        */
        void onDraw(SkCanvas* canvas) override;
        
        void setPlaceholderText(const std::string& text);
        [[nodiscard]] const std::string& getPlaceholderText() const;
        
        void setPlaceholderColor(const Color& color);
        [[nodiscard]] const Color& getPlaceholderColor() const;
        
        void setTextColor(const Color& color);
        [[nodiscard]] const Color& getTextColor() const;
        
        void setPosHint(const Point& pos) override;
        
    private:
        void onTextUpdate() override;
        void updateBounds();

    private:
        std::string m_placeholderText{};
        Vec2f m_textPos{0.0f, 0.0f};
        Color m_textColor{0xFFFFFFFF};
        Color m_placeholderColor{0xF0F0F0FF};
        float m_cursorHeight{0.0f};
        bool m_drawCursor{false};
    };
}

#endif // PTK_WIDGETS_TEXTFIELD_HPP

