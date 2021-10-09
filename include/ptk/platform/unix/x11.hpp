//
//  platform/unix/x11.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-12-28.
//

#ifndef PTK_PlATFORM_UNIX_X11_HPP
#define PTK_PlATFORM_UNIX_X11_HPP

extern "C" {
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xresource.h>
#include <X11/Xutil.h>

#undef None
#undef Status
}

namespace x11 {
    static constexpr long None = 0L;
}

#endif // PTK_PlATFORM_UNIX_X11_HPP