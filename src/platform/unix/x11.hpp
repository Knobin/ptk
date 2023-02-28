//
//  platform/unix/x11.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-12-28.
//

#ifndef PTK_PLATFORM_UNIX_X11_HPP
#define PTK_PLATFORM_UNIX_X11_HPP

extern "C"
{
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xresource.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xrandr.h>

#undef None
#undef Status
#undef Success
}

namespace x11
{
    static constexpr long None = 0L;
    using Status = int;
} // namespace x11

#endif // PTK_PLATFORM_UNIX_X11_HPP
