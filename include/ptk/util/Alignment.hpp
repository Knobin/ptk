//
//  util/Layout.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-07-29s.
//

#ifndef PTK_UTIL_ALIGNMENT_HPP
#define PTK_UTIL_ALIGNMENT_HPP

// pTK Headers
#include "ptk/core/Defines.hpp"
#include "ptk/core/Types.hpp"

// C++ Headers
#include <type_traits>

namespace pTK
{
    enum class Align : int32
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

        e.g. Align align{Align::Top | Align::Left};
        IsAlignSet(align, Align::Top); => returns true
        IsAlignSet(align, Align::Left); => returns true
        IsAlignSet(align, Align::Top, Align::Left); => returns true
        IsAlignSet(align, Align::Left, Align::Top); => returns true
        all other combinations will return false.
    */
    template<typename... Aligns>
    constexpr bool IsAlignSet(std::underlying_type<Align>::type number, Aligns&&... aligns) noexcept
    {
        using align_utype = std::underlying_type<Align>::type;
        align_utype align = 0;

        for (const auto p : { aligns... })
            align |= static_cast<align_utype>(p);

        return ((number & align) == align);
    }

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

    class PTK_API Alignment
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
        template<typename... Aligns>
        void setAlign(Aligns&&... aligns)
        {
            using align_utype = std::underlying_type<Align>::type;
            align_utype align = 0;

            for (const auto p : { aligns... })
                align |= static_cast<align_utype>(p);

            m_align = align;
            onAlignChange(m_align);
        }

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

    private:
        /** Function for notifying that Align property has changed.

            @param align    new Align value
        */
        virtual void onAlignChange(std::underlying_type<Align>::type) {}

        /** Function for notifying that Margin property has changed.

            @param margin    new Margin value
        */
        virtual void onMarginChange(const Margin&) {}

    private:
        Margin m_margin;
        std::underlying_type<Align>::type m_align;
    };
}

#endif // PTK_UTIL_ALIGNMENT_HPP
