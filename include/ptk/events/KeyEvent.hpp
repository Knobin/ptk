//
//  events/KeyEvent.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-12.
//

#ifndef PTK_EVENTS_KEYEVENT_HPP
#define PTK_EVENTS_KEYEVENT_HPP

// pTK Headers
#include "ptk/core/Event.hpp"
#include "ptk/core/Types.hpp"
#include "ptk/events/KeyCodes.hpp"
#include "ptk/core/Text.hpp"

// C++ Headers
#include <type_traits>

namespace pTK
{
    /** KeyEvent class implementation.

        Derived from Event, this class if for creating
        events from the keyboard. Such as Key presses or
        releases.
    */
    class PTK_API KeyEvent : public Event
    {
    public:
        // Objects for easier type management.
        inline static const Event::Type Pressed = Event::Type::KeyPressed;
        inline static const Event::Type Released = Event::Type::KeyReleased;

        enum class Modifier : byte
        {
            NONE        = 0,
            Shift       = 1,
            Control     = 2,
            Alt         = 4,
            Super       = 8,
            CapsLock    = 16,
            NumLock     = 32
        };

    public:
        /** Constructs KeyEvent with default values with type and code.

            @param type     press or release
            @param code     associated keycode
            @return         default initialized KeyEvent
        */
        KeyEvent(Event::Type t_type, KeyCode code, uint32 ch, std::underlying_type<KeyEvent::Modifier>::type mod = 0)
            : Event(Event::Category::Keyboard, t_type), keycode{code}, data{ch}, modifier{mod}
        {}

        /** Constructs KeyEvent with default values with type and code.

            @param type     press or release
            @param code     associated keycode
            @return         default initialized KeyEvent
        */
        KeyEvent(Event::Type t_type, KeyCode code, uint32 ch, Text::Encoding enc, std::underlying_type<KeyEvent::Modifier>::type mod = 0)
            : Event(Event::Category::Keyboard, t_type), keycode{code}, data{ch}, encoding{enc}, modifier{mod}
        {}

        // Key code.
        KeyCode keycode;

        // Contains the raw data (may be different depending on platform).
        uint32 data;

        // Contains the encoding of the char.
        Text::Encoding encoding{ Text::Encoding::UTF8 };

        // Modifiers.
        std::underlying_type<Modifier>::type modifier;

        bool isModifierSet(KeyEvent::Modifier mod) const;
    };

    /** InputEvent class implementation.

        Signals keyboard or text input.
    */
    class PTK_API InputEvent : public Event
    {
    public:
        using data_type = uint32;
        using data_cont = std::unique_ptr<data_type[]>;

    public:
        /** Constructs KeyEvent with default values with type and code.

            @param type     press or release
            @param code     associated keycode
            @return         default initialized KeyEvent
        */
        InputEvent(std::unique_ptr<uint32[]>& arr, std::size_t count, Text::Encoding enc = Text::Encoding::UTF8)
            : Event(Event::Category::Keyboard, Event::Type::KeyInput), data(std::move(arr)), size{count}, encoding(enc)
        {}

        // Contains array of characters.
        std::unique_ptr<uint32[]> data;

        // Number of characters.
        std::size_t size;

        // Contains the encoding of the array.
        Text::Encoding encoding;
    };

    constexpr bool IsKeyEventModifierSet(std::underlying_type<KeyEvent::Modifier>::type number, KeyEvent::Modifier mod)
    {
        using utype = std::underlying_type<KeyEvent::Modifier>::type;
        return (static_cast<utype>(mod) & number);
    }

    inline bool KeyEvent::isModifierSet(KeyEvent::Modifier mod) const
    {
        return IsKeyEventModifierSet(modifier, mod);
    }

    constexpr KeyEvent::Modifier KeyCodeToKeyEventModifier(const KeyCode code)
    {
        KeyEvent::Modifier mod = KeyEvent::Modifier::NONE;

        switch (code) {
            case pTK::Key::LeftShift:
            case pTK::Key::RightShift:
                mod = KeyEvent::Modifier::Shift;
                break;
            case pTK::Key::LeftControl:
            case pTK::Key::RightControl:
                mod = KeyEvent::Modifier::Control;
                break;
            case pTK::Key::LeftAlt:
            case pTK::Key::RightAlt:
                mod = KeyEvent::Modifier::Alt;
                break;
            case pTK::Key::LeftSuper:
            case pTK::Key::RightSuper:
                mod = KeyEvent::Modifier::Super;
                break;
            default:
                break;
        }

        return mod;
    }
}

#endif // PTK_EVENTS_KEYEVENT_HPP
