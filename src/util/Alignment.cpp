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
        : m_align{Align::Center}, m_margin{}, m_padding{}
    {

    }

    bool isAlignSet(std::underlying_type<Align>::type number, Align align)
    {
        return ((number & align) == align);
    }

    void Alignment::setAlign(std::underlying_type<Align>::type alignment)
    {
        m_align = alignment;
        onAlignChange(m_align);
    }

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

    void Alignment::setPadding(const Padding& padding)
    {
        m_padding = padding;
        onPaddingChange(m_margin);
    }

    void Alignment::setPaddingTop(Padding::value_type topPadding)
    {
        m_padding.top = topPadding;
        onPaddingChange(m_margin);
    }

    void Alignment::setPaddingBottom(Padding::value_type bottomPadding)
    {
        m_padding.bottom = bottomPadding;
        onPaddingChange(m_margin);
    }

    void Alignment::setPaddingLeft(Padding::value_type leftPadding)
    {
        m_padding.left = leftPadding;
        onPaddingChange(m_margin);
    }

    void Alignment::setPaddingRight(Padding::value_type rightPadding)
    {
        m_padding.right = rightPadding;
        onPaddingChange(m_margin);
    }

    void Alignment::setPaddingTopBottom(Padding::value_type topPadding, Padding::value_type bottomPadding)
    {
        m_padding.top = topPadding;
        m_padding.bottom = bottomPadding;
        onPaddingChange(m_margin);
    }

    void Alignment::setPaddingLeftRight(Padding::value_type leftPadding, Padding::value_type rightPadding)
    {
        m_padding.left = leftPadding;
        m_padding.right = rightPadding;
        onPaddingChange(m_margin);
    }

    const Margin& Alignment::getPadding() const
    {
        return m_padding;
    }

    Padding::value_type Alignment::getPaddingTop() const
    {
        return m_padding.top;
    }

    Padding::value_type Alignment::getPaddingBottom() const
    {
        return m_padding.bottom;
    }

    Padding::value_type Alignment::getPaddingLeft() const
    {
        return m_padding.left;
    }

    Padding::value_type Alignment::getPaddingRight() const
    {
        return m_padding.right;
    }
}
