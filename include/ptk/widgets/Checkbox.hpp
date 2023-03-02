//
//  widgets/Checkbox.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-09-11.
//

#ifndef PTK_WIDGETS_CHECKBOX_HPP
#define PTK_WIDGETS_CHECKBOX_HPP

// pTK Headers
#include "ptk/core/Widget.hpp"

namespace pTK
{
    /** Checkbox class implementation.

        This class implements a basic Checkbox.
        It includes all the necessary function for
        it to work.

        Checkbox specific functions implemented are:
            - bool status();
            - void set(bool status):
            - bool toggle();

        An extra callback is also present for overriding when
        the checkbox is toggled.
    */
    class PTK_API Checkbox : public Widget
    {
    public:
        /** Checkbox with default values.

            @return    default initialized Checkbox
        */
        Checkbox();

        /** Move Constructor for Checkbox.

            @return    initialized Checkbox from value
        */
        Checkbox(Checkbox&& other) = default;

        /** Move Assignment operator for Checkbox.

            @return    Checkbox with value
        */
        Checkbox& operator=(Checkbox&& other) = default;

        /** Destructor for Checkbox.

        */
        virtual ~Checkbox() = default;

        /** Draw function.
            Function is called when it is time to draw.

            Derived from Drawable.

            @param canvas  Canvas to draw to
        */
        void onDraw(SkCanvas* canvas) override;

        /** Function for retrieving the status.

            @return    status
        */
        [[nodiscard]] bool status() const;

        /** Function for setting the status.

            @param status  he status toggled to.
        */
        void set(bool status);

        /** Function for toggling the status.

            @return    the status toggled to.
        */
        bool toggle();

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

        /** Function for handling when mouse is entering.

            Derived from EventFunctions.
        */
        void onEnterCallback(const EnterEvent& evt);

        /** Function for handling when mouse is leaving.

            Derived from EventFunctions.
        */
        void onLeaveCallback(const LeaveEvent& evt);

        /** Function for handling when mouse is clicking.

            Derived from EventFunctions.

            @param button      which button on mouse triggered the event.
            @param position    x and y position
        */
        void onClickCallback(const ClickEvent& evt);

        /** Function for handling when mouse is released.

            Derived from EventFunctions.

            @param button      which button on mouse triggered the event.
            @param position    x and y position
        */
        void onReleaseCallback(const ReleaseEvent& evt);

        /** Function for handling when the checkbox is toggled.

            @param status    status on toggle.
        */
        virtual void onToggleEvent(bool status);

        /** Function for handling when the checkbox is toggled.

            @param callback    function to call on toggle.
        */
        void onToggle(const std::function<bool(bool status)>& callback);

    private:
        void initCallbacks();

    private:
        bool m_checked{false};
        bool m_hover{false};
        bool m_click{false};
        uint8_t m_state = 0;
        Color m_checkColor{0x007BFFFF};
        std::function<bool(bool status)> m_toggleCallback{nullptr};
        float m_cornerRadius{0.0f};
        Color m_color{0x00000000};
        Color m_outlineColor{0xf5f5f5ff};
        float m_outlineThickness{0.0f};

        void drawChecked(SkCanvas* canvas);
        void drawStates(SkCanvas* canvas);

        void internalToggle();
    };

} // namespace pTK

#endif // PTK_WIDGETS_CHECKBOX_HPP
