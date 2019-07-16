//
//  widgets/Button.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-07-15.
//

#ifndef PTK_WIDGETS_BUTTON_HPP
#define PTK_WIDGETS_BUTTON_HPP

// Local Headers
#include "ptk/widgets/Label.hpp"
#include "ptk/widgets/Rectangle.hpp"

namespace pTK
{
    /** Button class implementation.
     
     This class is a simple button.
     Unique elements is different colors depending on event.
     Such as hover and click colors.
     */
    class Button : public Rectangle
    {
    public:
        struct Style
        {
            Color color;
            Color hoverColor;
            Color clickColor;
            Color textColor;
            uint cornerRadius;
            
            static const Style Default;
            static const Style Success;
            static const Style Danger;
        };
        
    public:
        /** Constructs Button with default values.
         
         @return    default initialized Button
         */
        Button();
        
        /** Constructs Button with default values with style.
         
         @return    default initialized Button
         */
        Button(const Style& style);
        virtual ~Button() = default;
        
        /** Function for setting the position of the Button.
         
         Derived from Widget.
         
         @param pos     Position to set
         */
        void setPosHint(const Position& pos) override;
        
        /** Draw function.
         Function is called when it is time to draw.
         
         Derived from Drawable.
         @param canvas  canvas to draw to
         */
        void onDraw(SkCanvas* canvas) override;
        
        /** Function for a child Widget to call the Parent when it
         need to be drawn.
         
         Derived from Widget.
         
         @param widget  Child Widget
         @return        Status (true if handled)
         */
        bool drawChild(Widget* widget) override;
        
        /** Function for setting the text of the Label
         
         @param text    text for Label
         */
        void setText(const std::string& text);
        
        /** Function for retrieving the current text used by Label.
         
         @return  current text
         */
        const std::string& getText() const;
        
        /** Function for setting the font family of the Label.
         
         Derived from Font.
         
         @param font    font family for Label to use
         */
        void setFontFamily(const std::string& fontFamily);
        
        /** Function for retrieving the current font family used by Label.
         
         @return  current font family
         */
        const std::string& getFontFamily() const;
        
        /** Function for setting the font size of the Label.
         
         Derived from Font.
         
         @param fontSize    size of the font
         */
        void setFontSize(uint fontSize);
        
        /** Function for retrieving the current font size used by Label.
         
         @return  current font size
         */
        uint getFontSize() const;
        
        /** Function for setting the Color of the text.
         
         @param textColor   Text Color
         */
        void setTextColor(const Color& textColor);
        
        /** Function for retrieving the Color of the text.
         
         @return    Current hover color
         */
        const Color& getTextColor() const;
        
        /** Function for setting a Label to the Button.
         
         @param label   New label
         */
        void setLabel(const std::shared_ptr<Label> label);
        
        /** Function for retrieving the Label of the Button.
         
         @return    Current set label
         */
        std::shared_ptr<Label> getLabel() const;
        
        /** Function for setting the Hover Color of the Button.
         
         @param hoverColor   Hover Color
         */
        void setHoverColor(const Color& hoverColor);
        
        /** Function for retrieving the Hover Color of the Button.
         
         @return    Current hover color
         */
        const Color& getHoverColor() const;
        
        /** Function for setting the Click Color of the Button.
         
         @param hoverColor   Click Color
         */
        void setClickColor(const Color& clickColor);
        
        /** Function for retrieving the Click Color of the Button.
         
         @return    Current click color
         */
        const Color& getClickColor() const;
        
        /** Function for setting the margin to the text.
         
         @param margin   text margin
         */
        void setMargin(uint margin);
        
        /** Function for retrieving the margin to the text.
         
         @return    Current margin
         */
        uint getMargin() const;
        
        /** Function for setting the style of the Button.
         
         @param margin   text margin
         */
        void setStyle(const Style& style);
        
        /** Function for retrieving the style of the Button.
         
         @return    Current margin
         */
        Style getStyle() const;
        
        /** Function for handling when mouse is entering.
         
         Derived from EventFunctions.
         */
        bool onEnterEvent() override;
        
        /** Function for handling when mouse is leaving.
         
         Derived from EventFunctions.
         */
        bool onLeaveEvent() override;
        
        /** Function for handling when mouse is clicking.
         
         Derived from EventFunctions.
         
         @param button      which button on mouse triggered the event.
         @param position    x and y position
         */
        bool onClickEvent(MouseButton, const Position&) override;
        
        /** Function for handling when mouse is released.
         
         Derived from EventFunctions.
         
         @param button      which button on mouse triggered the event.
         @param position    x and y position
         */
        bool onReleaseEvent(MouseButton, const Position&) override;
        
    private:
        std::shared_ptr<Label> m_text;
        Position m_labelPos;
        Size m_labelSize;
        uint m_margin;
        Color m_hoverColor;
        Color m_clickColor;
        Color m_colorCopy;
        bool m_hover;
        bool m_click;
    };
}


#endif // PTK_WIDGETS_BUTTON_HPP

