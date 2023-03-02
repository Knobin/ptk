//
//  widgets/Label.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-30.
//

#ifndef PTK_WIDGETS_LABEL_HPP
#define PTK_WIDGETS_LABEL_HPP

// pTK Headers
#include "ptk/core/Text.hpp"
#include "ptk/core/Widget.hpp"
#include "ptk/util/Color.hpp"

namespace pTK
{
    /** Label class implementation.

        This class is for drawing a Label.
    */
    class PTK_API Label : public Widget, public Text
    {
    public:
        /** Constructs Label with default values.

            @return    default initialized Label
        */
        Label() = default;

        /** Move Constructor for Label.

            @return    initialized Label from value
        */
        Label(Label&& other) = default;

        /** Move Assignment operator for Label.

            @return    Label with value
        */
        Label& operator=(Label&& other) = default;

        /** Destructor for Label.

        */
        virtual ~Label() = default;

        /** Draw function.
            Function is called when it is time to draw.

            Derived from Drawable.
        */
        void onDraw(SkCanvas* canvas) override;

        /** Function for setting the text.

            Note: Will apply the new text bounds as size and min/max sizes.

            @param str      new text
        */
        void setText(const std::string& str);

        /** Function for retrieving current set text.

            @return    text
        */
        [[nodiscard]] const std::string& getText() const noexcept;

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
        // Label should only change size on setFontSize!
        using Widget::setMaxSize;
        using Widget::setMinSize;
        using Widget::setSize;

        void onTextUpdate() override;

    private:
        std::string m_text{}; // Only supports UTF-8 for now.
        Color m_color{0xf5f5f5ff};
        Color m_outlineColor{0xf5f5f5ff};
        float m_outlineThickness{0.0f};
    };
} // namespace pTK

#endif // PTK_WIDGETS_LABEL_HPP
