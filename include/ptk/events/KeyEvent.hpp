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
    class KeyEvent : public Event
    {
    public:
        // Objects for easier type management.
        inline static const Event::Type Pressed = Event::Type::KeyPressed;
        inline static const Event::Type Released = Event::Type::KeyReleased;
        inline static const Event::Type Input = Event::Type::KeyInput;
        
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
        KeyEvent(Event::Type type, KeyCode code, uint32 ch, std::underlying_type<KeyEvent::Modifier>::type mod = 0)
            : Event(Event::Category::Key, type), keycode{ code }, data{ ch }, modifier{ mod }
        {}

        /** Constructs KeyEvent with default values with type and code.

            @param type     press or release
            @param code     associated keycode
            @return         default initialized KeyEvent
        */
        KeyEvent(Event::Type type, KeyCode code, uint32 ch, Text::Encoding enc, std::underlying_type<KeyEvent::Modifier>::type mod = 0)
            : Event(Event::Category::Key, type), keycode{ code }, data{ ch }, encoding{ enc }, modifier{ mod }
        {}

        // Key code.
        KeyCode keycode;

        // Contains the char.
        uint32 data;

        // Contains the encoding of the char.
        Text::Encoding encoding{ Text::Encoding::UTF8 };
        
        // Modifiers.
        std::underlying_type<Modifier>::type modifier;
        
        bool isModifierSet(KeyEvent::Modifier mod) const;
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
