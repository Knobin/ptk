//
//  ptk.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-08-31.
//

#ifndef PTK_HPP
#define PTK_HPP

// --- Application -------------------
#include "ptk/Application.hpp"
#include "ptk/Core.hpp"
#include "ptk/Window.hpp"

// --- Core --------------------------
#include "ptk/core/CallbackStorage.hpp"
#include "ptk/core/Defines.hpp"
#include "ptk/core/Drawable.hpp"
#include "ptk/core/Event.hpp"
#include "ptk/core/EventCallbacks.hpp"
#include "ptk/core/EventFunctions.hpp"
#include "ptk/core/EventHandling.hpp"
#include "ptk/core/EventManager.hpp"
#include "ptk/core/EventQueue.hpp"
#include "ptk/core/Exception.hpp"
#include "ptk/core/Shape.hpp"
#include "ptk/core/Sizable.hpp"
#include "ptk/core/Text.hpp"
#include "ptk/core/Types.hpp"
#include "ptk/core/Widget.hpp"
#include "ptk/core/WidgetContainer.hpp"
#include "ptk/core/WindowInfo.hpp"

// --- Events ------------------------
#include "ptk/events/KeyCodes.hpp"
#include "ptk/events/KeyEvent.hpp"
#include "ptk/events/KeyMap.hpp"
#include "ptk/events/MouseEvent.hpp"
#include "ptk/events/WidgetEvents.hpp"
#include "ptk/events/WindowEvent.hpp"

// --- Menu --------------------------
#include "ptk/menu/CheckboxMenuItem.hpp"
#include "ptk/menu/Menu.hpp"
#include "ptk/menu/MenuBar.hpp"
#include "ptk/menu/MenuItem.hpp"
#include "ptk/menu/MenuItemSeparator.hpp"
#include "ptk/menu/MenuItemType.hpp"
#include "ptk/menu/NamedMenuItem.hpp"
#include "ptk/menu/Shortcut.hpp"

// --- Util --------------------------
#include "ptk/util/Alignment.hpp"
#include "ptk/util/Color.hpp"
#include "ptk/util/IterableContainer.hpp"
#include "ptk/util/Math.hpp"
#include "ptk/util/NonCopyable.hpp"
#include "ptk/util/NonMovable.hpp"
#include "ptk/util/Point.hpp"
#include "ptk/util/SafeQueue.hpp"
#include "ptk/util/Semaphore.hpp"
#include "ptk/util/SingleObject.hpp"
#include "ptk/util/Size.hpp"
#include "ptk/util/SizePolicy.hpp"
#include "ptk/util/Vec2.hpp"

// --- Widgets -----------------------
#include "ptk/widgets/Button.hpp"
#include "ptk/widgets/Checkbox.hpp"
#include "ptk/widgets/HBox.hpp"
#include "ptk/widgets/Image.hpp"
#include "ptk/widgets/Label.hpp"
#include "ptk/widgets/Rectangle.hpp"
#include "ptk/widgets/TextField.hpp"
#include "ptk/widgets/VBox.hpp"


#endif // PTK_HPP
