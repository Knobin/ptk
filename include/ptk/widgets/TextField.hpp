//
//  widgets/TextField.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-04-10.
//

#ifndef PTK_WIDGETS_TEXTFIELD_HPP
#define PTK_WIDGETS_TEXTFIELD_HPP

// pTK Headers
#include "ptk/core/Text.hpp"
#include "ptk/core/Widget.hpp"
#include "ptk/util/Vec2.hpp"

namespace pTK
{
    /** TextField class implementation.

    */
    class PTK_API TextField : public Widget, public Text
    {
    public:
        /** Constructs TextField with default values.

            @return    default initialized TextField
        */
        TextField();

        /** Move Constructor for TextField.

            @return    initialized TextField from value
        */
        TextField(TextField&& other) = default;

        /** Move Assignment operator for TextField.

            @return    TextField with value
        */
        TextField& operator=(TextField&& other) = default;

        /** Destructor for TextField.

        */
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

        /** Function for setting the text.

            Note: Will apply the new text bounds as size and min/max sizes.

            @param str      new text
        */
        void setText(const std::string& text);

        /** Function for retrieving current set text.

            @return    text
        */
        [[nodiscard]] const std::string& getText() const;

        /** Function for setting the corner radius.

            @param radius  corner radius
        */
        void setCornerRadius(float radius);

        /** Function for retrieving the corner radius.

            @return    corner radius
        */
        [[nodiscard]] float getCornerRadius() const;

        /** Function for retrieving the Color of the Shape.

            @return    Current Color
        */
        [[nodiscard]] const Color& getColor() const;

        /** Function for setting the Color of the Shape.

            @param Color   Shape Color
        */
        virtual void setColor(const Color& color);

        /** Function for retrieving the Color of the Shape.

            @return    Current Color
        */
        [[nodiscard]] const Color& getOutlineColor() const;

        /** Function for setting the Color of the outline.

            @param outline_color   outline Color
        */
        virtual void setOutlineColor(const Color& outlineColor);

        /** Function for retrieving the thickness of the outline.

            @return    outline thickness
        */
        [[nodiscard]] float getOutlineThickness() const;

        /** Function for setting the thickness of the outline.

            @param outline_thickness   thickness of outline
        */
        virtual void setOutlineThickness(float outlineThickness);

    private:
        // Callback function from Text.
        void onTextUpdate() override;

        // Updates the bounds, font member variables and minimal size.
        void updateBounds();

        // Handles for keyboard input.
        void handleKeyPress(KeyCode keycode, uint8_t modifier);
        void removeFromText(int direction);
        void moveCursor(int direction, std::size_t strSize, bool shouldDraw = false);
        void moveCursorToPos(std::size_t pos, std::size_t strSize, bool shouldDraw = false);

        void handleInput(const std::unique_ptr<uint32_t[]>& data, std::size_t size, Text::Encoding encoding);

    private:
        std::string m_placeholderText{};
        Vec2f m_textPos{0.0f, 0.0f};
        Color m_textColor{0xFFFFFFFF};
        Color m_placeholderColor{0xF0F0F0FF};
        float m_cursorHeight{0.0f};
        std::size_t m_cursorLocation{0};
        bool m_drawCursor{false};

        float m_cornerRadius{0.0f};
        Color m_color{0xf5f5f5ff};
        Color m_outlineColor{0xf5f5f5ff};
        float m_outlineThickness{0.0f};

        // Only supports UTF-8 for now.
        std::string m_text{};
    };
} // namespace pTK

#endif // PTK_WIDGETS_TEXTFIELD_HPP
