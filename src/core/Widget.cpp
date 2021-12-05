//
//  core/Widget.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-19.
//

// pTK Headers
#include "ptk/core/Widget.hpp"

namespace pTK
{
    Widget::Widget()
        : Alignment(), Drawable(), EventHandling(), Sizable(),
        m_parent{nullptr}, m_name{}
    {

    }

    void Widget::setParent(Widget* parent)
    {
        m_parent = parent;
    }

    Widget* Widget::getParent() const
    {
        return m_parent;
    }

    void Widget::onSizeChange(const Size&)
    {
        update();
    }

    void Widget::onLimitChange(const Size&, const Size&)
    {
        update();
    }

    void Widget::setPosHint(const Point& pos)
    {
        m_pos = pos;
        update();
    }

    const Point& Widget::getPosition() const
    {
        return m_pos;
    }

    void Widget::setName(const std::string& name)
    {
        m_name = name;
    }

    const std::string& Widget::getName() const
    {
        return m_name;
    }

    bool Widget::updateChild(Widget*)
    {
        return true;
    }

    bool Widget::update()
    {
        if (m_parent != nullptr)
            return m_parent->updateChild(this);

        return false;
    }

    bool Widget::drawChild(Widget*)
    {
        return true;
    }

    bool Widget::draw()
    {
        if (m_parent != nullptr)
            return m_parent->drawChild(this);

        return false;
    }

    void Widget::show()
    {
        Drawable::show(); // Set the visible boolean.
        update(); // Tell parent to display widget.
    }

    void Widget::hide()
    {
        Drawable::hide(); // Set the visible boolean.
        update(); // Tell parent to hide widget.
    }

    Size Widget::getOuterSize() const
    {
        return calcOuterFromSize(getSize());
    }

    Size Widget::calcOuterFromSize(const Size& size) const
    {
        const Margin margin{getMargin()};
        const Margin::value_type hMargin{AddWithoutOverflow(margin.left, margin.right)};
        const Margin::value_type vMargin{AddWithoutOverflow(margin.top, margin.bottom)};

        return {AddWithoutOverflow(size.width, hMargin),
                AddWithoutOverflow(size.height, vMargin)};
    }

    void Widget::onAlignChange(std::underlying_type<Align>::type)
    {
        update();
    }

    void Widget::onMarginChange(const Margin&)
    {
        update();
    }

    ///////////////////////////////////////////////////////////////////////////////

    // Comparison operators.
    bool operator==(const Widget& lhs, const Widget& rhs)
    {
        return ((lhs.getPosition() == rhs.getPosition()) &&
                (lhs.getSize() == rhs.getSize()) &&
                (lhs.getName() == rhs.getName()) &&
                (lhs.getParent() == rhs.getParent()));
    }

    bool operator!=(const Widget& lhs, const Widget& rhs)
    {
        return !(lhs == rhs);
    }
}
