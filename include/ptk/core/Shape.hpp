//
//  core/Shape.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-19.
//

#ifndef PTK_CORE_SHAPE_HPP
#define PTK_CORE_SHAPE_HPP

// pTK Headers
#include "ptk/core/Widget.hpp"
#include "ptk/util/Color.hpp"

namespace pTK
{
    class PTK_API Shape : public Widget
    {
    public:
        /** Constructs Shape with default values.

            @return    default initialized Shape
        */
        Shape();

        /** Move Constructor for Shape.

            @return    initialized Shape from value
        */
        Shape(Shape&& other) = default;

        /** Move Assignment operator for Shape.

            @return    Shape with value
        */
        Shape& operator=(Shape&& other) = default;

        /** Destructor for Shape.

        */
        virtual ~Shape() = default;

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
        Color m_color;
        Color m_outlineColor;
        float m_outlineThickness;
    };

    // Comparison operators.
    PTK_API bool operator==(const Shape& lhs, const Shape& rhs);
    PTK_API bool operator!=(const Shape& lhs, const Shape& rhs);
} // namespace pTK

#endif // PTK_CORE_SHAPE_HPP
