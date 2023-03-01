//
//  core/Widget.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-19.
//

// pTK Headers
#include "ptk/core/Widget.hpp"
#include "ptk/util/Math.hpp"

namespace pTK
{
    Widget::Widget()
        : Alignment(),
          Drawable(),
          EventHandling(),
          Sizable(),
          m_parent{nullptr},
          m_name{}
    {}

    Widget::~Widget()
    {}

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
        update();         // Tell parent to display widget.
    }

    void Widget::hide()
    {
        Drawable::hide(); // Set the visible boolean.
        update();         // Tell parent to hide widget.
    }

    Size Widget::getOuterSize() const
    {
        return calcOuterFromSize(getSize());
    }

    Size Widget::calcOuterFromSize(const Size& size) const
    {
        const Margin margin{getMargin()};
        const Margin::value_type hMargin{Math::AddWithoutOverflow(margin.left, margin.right)};
        const Margin::value_type vMargin{Math::AddWithoutOverflow(margin.top, margin.bottom)};

        return {Math::AddWithoutOverflow(size.width, hMargin), Math::AddWithoutOverflow(size.height, vMargin)};
    }

    SizePolicy Widget::getSizePolicy() const noexcept
    {
        return m_sizePolicy;
    }

    void Widget::setSizePolicy(SizePolicy policy)
    {
        m_sizePolicy = policy;
        update();
    }

    Limits Widget::getLimitsWithSizePolicy() const noexcept
    {
        Limits limits{getLimits()};
        const Size size{getSize()};
        const SizePolicy policy{getSizePolicy()};

        if (policy.horizontal == SizePolicy::Policy::Fixed)
        {
            limits.min.width = size.width;
            limits.max.width = size.width;
        }

        if (policy.vertical == SizePolicy::Policy::Fixed)
        {
            limits.min.height = size.height;
            limits.max.height = size.height;
        }

        return limits;
    }

    ///////////////////////////////////////////////////////////////////////////////

    // Comparison operators.
    bool operator==(const Widget& lhs, const Widget& rhs)
    {
        return ((lhs.getPosition() == rhs.getPosition()) && (lhs.getSize() == rhs.getSize()) &&
                (lhs.getName() == rhs.getName()) && (lhs.getParent() == rhs.getParent()));
    }

    bool operator!=(const Widget& lhs, const Widget& rhs)
    {
        return !(lhs == rhs);
    }
} // namespace pTK
