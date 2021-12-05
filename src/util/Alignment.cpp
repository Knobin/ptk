//
//  util/Align.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-07-29s.
//

// pTK Headers
#include "ptk/util/Alignment.hpp"

namespace pTK
{
    Alignment::Alignment()
        :  m_margin{}, m_align{static_cast<std::underlying_type<Align>::type>(Align::Center)}
    {}

    std::underlying_type<Align>::type Alignment::getAlign() const
    {
        return m_align;
    }

    void Alignment::setMargin(const Margin& margin)
    {
        m_margin = margin;
        onMarginChange(m_margin);
    }

    void Alignment::setMarginTop(Margin::value_type topMargin)
    {
        m_margin.top = topMargin;
        onMarginChange(m_margin);
    }

    void Alignment::setMarginBottom(Margin::value_type bottomMargin)
    {
        m_margin.bottom = bottomMargin;
        onMarginChange(m_margin);
    }

    void Alignment::setMarginLeft(Margin::value_type leftMargin)
    {
        m_margin.left = leftMargin;
        onMarginChange(m_margin);
    }

    void Alignment::setMarginRight(Margin::value_type rightMargin)
    {
        m_margin.right = rightMargin;
        onMarginChange(m_margin);
    }

    void Alignment::setMarginTopBottom(Margin::value_type topMargin, Margin::value_type bottomMargin)
    {
        m_margin.top = topMargin;
        m_margin.bottom = bottomMargin;
        onMarginChange(m_margin);
    }

    void Alignment::setMarginLeftRight(Margin::value_type leftMargin, Margin::value_type rightMargin)
    {
        m_margin.left = leftMargin;
        m_margin.right = rightMargin;
        onMarginChange(m_margin);
    }

    const Margin& Alignment::getMargin() const
    {
        return m_margin;
    }

    Margin::value_type Alignment::getMarginTop() const
    {
        return m_margin.top;
    }

    Margin::value_type Alignment::getMarginBottom() const
    {
        return m_margin.bottom;
    }

    Margin::value_type Alignment::getMarginLeft() const
    {
        return m_margin.left;
    }

    Margin::value_type Alignment::getMarginRight() const
    {
        return m_margin.right;
    }
}
