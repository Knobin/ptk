//
//  util/Layout.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-07-29s.
//

#ifndef PTK_UTIL_ALIGNMENT_HPP
#define PTK_UTIL_ALIGNMENT_HPP

// pTK Headers
#include "ptk/core/Types.hpp"

// C++ Headers
#include <type_traits>

namespace pTK
{
    enum Align : int32
    {
        Top     = 0x0001,
        Bottom  = 0x0002,
        Left    = 0x0004,
        Right   = 0x0008,

        HCenter = 0x0020,
        VCenter = 0x0040,
        Center  = 0x0080
    };
    
    /** Function for checking if a specific Align is set.

        e.g. Align align{Align::Top | Align::Bottom};
        isAlignSet(align, Align::Top); => returns true
        isAlignSet(align, Align::Bottom); => returns true
        isAlignSet(align, Align::Top | Align::Bottom); => returns true
        all other combinations will return false.
    */
    bool isAlignSet(std::underlying_type<Align>::type number, Align align);

    /** Directions struct implementation.
     
        Pretty self-explanatory.
    */
    template<typename T>
    struct Directions
    {
        using value_type = T;

        value_type top;
        value_type bottom;
        value_type left;
        value_type right;
    };
    
    using Margin = Directions<uint32>;
    using Padding = Directions<uint32>;

    class Alignment
    {
    public:
        /** Constructs Alignment with default values.

            @return    default initialized Alignment
        */
        Alignment();

        /** Destructor for Alignment.

        */
        virtual ~Alignment() = default;

        /** Function for setting the align property of the Widget.

            @param align  Align to apply.
        */
        void setAlign(std::underlying_type<Align>::type align);

        /** Function for retrieving the align property of the Widget.

            @return current align
        */
        [[nodiscard]] std::underlying_type<Align>::type getAlign() const;

        /** Function for setting the margin of the Widget.

            @param margin  Margin to apply.
        */
        void setMargin(const Margin& margin);

        /** Function for setting the top margin of the Widget.

            @param topMargin  value to apply for top margin.
        */
        void setMarginTop(Margin::value_type topMargin);

        /** Function for setting the bottom margin of the Widget.

            @param bottomMargin  value to apply for bottom margin.
        */
        void setMarginBottom(Margin::value_type bottomMargin);

        /** Function for setting the left margin of the Widget.

            @param leftMargin  value to apply for left margin.
        */
        void setMarginLeft(Margin::value_type leftMargin);

        /** Function for setting the right margin of the Widget.

            @param rightMargin  value to apply for right margin.
        */
        void setMarginRight(Margin::value_type rightMargin);

        /** Function for setting the top and bottom margin of the Widget.

            @param topMargin  value to apply for top margin.
            @param bottomMargin  value to apply for bottom margin.
        */
        void setMarginTopBottom(Margin::value_type topMargin, Margin::value_type bottomMargin);

        /** Function for setting the left and right margin of the Widget.

            @param leftMargin  value to apply for left margin.
            @param rightMargin  value to apply for right margin.
        */
        void setMarginLeftRight(Margin::value_type leftMargin, Margin::value_type rightMargin);

        /** Function for retrieving the margin of the Widget.

            @return  current margin
        */
        [[nodiscard]] const Margin& getMargin() const;

        /** Function for retrieving the top margin of the Widget.

            @return  current top margin
        */
        [[nodiscard]] Margin::value_type getMarginTop() const;

        /** Function for retrieving the bottom margin of the Widget.

            @return  current bottom margin
        */
        [[nodiscard]] Margin::value_type getMarginBottom() const;

        /** Function for retrieving the left margin of the Widget.

            @return  current left margin
        */
        [[nodiscard]] Margin::value_type getMarginLeft() const;

        /** Function for retrieving the right margin of the Widget.

            @return  current right margin
        */
        [[nodiscard]] Margin::value_type getMarginRight() const;

        /** Function for setting the padding of the Widget.

            @param padding  Padding to apply.
        */
        void setPadding(const Padding& padding);

        /** Function for setting the top padding of the Widget.

            @param topPadding  value to apply for top padding.
        */
        void setPaddingTop(Padding::value_type topPadding);

        /** Function for setting the bottom padding of the Widget.

            @param bottomPadding  value to apply for bottom padding.
        */
        void setPaddingBottom(Margin::value_type bottomPadding);

        /** Function for setting the left padding of the Widget.

            @param leftPadding  value to apply for left padding.
        */
        void setPaddingLeft(Padding::value_type leftPadding);

        /** Function for setting the right padding of the Widget.

            @param rightPadding  value to apply for right padding.
        */
        void setPaddingRight(Padding::value_type rightPadding);

        /** Function for setting the top and bottom padding of the Widget.

            @param topPadding  value to apply for top padding.
            @param bottomPadding  value to apply for bottom padding.
        */
        void setPaddingTopBottom(Padding::value_type topPadding, Padding::value_type bottomPadding);

        /** Function for setting the left and right padding of the Widget.

            @param leftPadding  value to apply for left padding.
            @param rightPadding  value to apply for right padding.
        */
        void setPaddingLeftRight(Padding::value_type leftPadding, Padding::value_type rightPadding);

        /** Function for retrieving the padding of the Widget.

            @return  current padding
        */
        [[nodiscard]] const Margin& getPadding() const;

        /** Function for retrieving the top padding of the Widget.

            @return  current top padding
        */
        [[nodiscard]] Padding::value_type getPaddingTop() const;

        /** Function for retrieving the bottom padding of the Widget.

            @return  current bottom padding
        */
        [[nodiscard]] Padding::value_type getPaddingBottom() const;

        /** Function for retrieving the left padding of the Widget.

            @return  current left padding
        */
        [[nodiscard]] Padding::value_type getPaddingLeft() const;

        /** Function for retrieving the right padding of the Widget.

            @return  current right padding
        */
        [[nodiscard]] Padding::value_type getPaddingRight() const;

    private:
        /** Function for notifying that Align property has changed.

            @param align    new Align value
        */
        virtual void onAlignChange(std::underlying_type<Align>::type) {}

        /** Function for notifying that Margin property has changed.

            @param margin    new Margin value
        */
        virtual void onMarginChange(const Margin&) {}

        /** Function for notifying that Padding property has changed.

            @param padding    new Padding value
        */
        virtual void onPaddingChange(const Padding&) {}

    private:
        std::underlying_type<Align>::type m_align;
        Margin m_margin;
        Padding m_padding;
    };
}

#endif // PTK_UTIL_ALIGNMENT_HPP
