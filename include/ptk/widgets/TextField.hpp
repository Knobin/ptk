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

        /** Function for setting the placeholder text.

            @param text    string to set
        */
        void setPlaceholderText(const std::string& text);

        /** Function for retrieving the placeholder text.

            @return  current text
        */
        [[nodiscard]] const std::string& getPlaceholderText() const;

        /** Function for setting the placeholder color.

            @param color    color to set
        */
        void setPlaceholderColor(const Color& color);

        /** Function for retrieving the placeholder color.

            @return  placeholder color
        */
        [[nodiscard]] const Color& getPlaceholderColor() const;

        /** Function for setting the text color.

            @param color    color to set
        */
        void setTextColor(const Color& color);

        /** Function for retrieving the text color.

            @return  text color
        */
        [[nodiscard]] const Color& getTextColor() const;

        /** Function for requesting the position of the Widget.

            @param pos  requested position of the Widget.
        */
        void setPosHint(const Point& pos) override;

        void setText(const std::string& text);
        [[nodiscard]] const std::string& getText() const;

        
    private:
        // Callback function from Text.
        void onTextUpdate() override;

        // Updates the bounds, font member variables and minimal size.
        void updateBounds();

        // Handles for keyboard input.
        void handleKeyPress(KeyCode keycode, byte modifier);
        void removeFromText(int direction);
        void moveCursor(int direction, std::size_t strSize, bool shouldDraw = false);
        void moveCursorToPos(std::size_t pos, std::size_t strSize, bool shouldDraw = false);

        void handleInput(KeyCode key, uint32 data, Text::Encoding encoding, byte modifier);

    private:
        std::string m_placeholderText{};
        Vec2f m_textPos{0.0f, 0.0f};
        Color m_textColor{0xFFFFFFFF};
        Color m_placeholderColor{0xF0F0F0FF};
        float m_cursorHeight{0.0f};
        std::size_t m_cursorLocation{0};
        bool m_drawCursor{false};

        // Only supports UTF-8 for now.
        std::string m_text{};
    };
}

#endif // PTK_WIDGETS_TEXTFIELD_HPP

