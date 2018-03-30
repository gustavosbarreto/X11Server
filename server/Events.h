#ifndef _EVENTS_H_
#define _EVENTS_H_

#include "types.h"

namespace X
{
    struct KeyEvent
    {
        KEYCODE     detail;
        uint16      seq;
        TIMESTAMP   timestamp;
        WINDOW      rootWnd, eventWnd, childWnd;
        int16       root_x, root_y, event_x, event_y;
        /* Set of keybut mask */
        BOOL        same_screen;
    };
    
    typedef KeyEvent KeyPressEvent;
    typedef KeyEvent KeyReleaseEvent;

    struct PointerEvent
    {
        BUTTON      button;
        uint16      seq;
        TIMESTAMP   timestamp;
        WINDOW rootWnd, eventWnd, childWnd;
        int16       root_x, root_y, event_x, event_y;
        /* Set of keybut mask */
        BITMASK     keybutmask;
        BOOL        same_screen;
    };

    typedef PointerEvent ButtonPressEvent;
    typedef PointerEvent ButtonReleaseEvent;
    typedef PointerEvent MotionNotifyEvent;

    struct CrossingEvent
    {
        enum Detail {Ancestor = 0, Virtual, Inferior, Nonlinear, NonlinearVirtual};
        Detail      detail;
        uint16      seq;
        TIMESTAMP   timestamp;
        WINDOW rootWnd, eventWnd, childWnd;
        int16       root_x, root_y, event_x, event_y;
        /* Set of keybut mask */
        BITMASK     keybutmask;
        byte        mode;
        BOOL        samescreen;
    };

    typedef CrossingEvent EnterNotifyEvent;
    typedef CrossingEvent LeaveNotifyEvent;

    struct FocusEvent
    {
        byte        detail;
        uint16      seq;
        WINDOW      ev;
        byte        mode;
    };

    typedef FocusEvent FocusInEvent;
    typedef FocusEvent FocusOutEvent;

    /*
     *
     */
    struct KeymapNotifyEvent
    {
        byte keys[31];
    };

    // MapNotify/UnmapNotify
    struct MapEvent
    {
        uint16 seq;
        WINDOW eventWnd, wnd;
        union
        {
            bool overrideRedirect;
            bool fromConfigure;
        };
    };

    struct MapRequestEvent
    {
        uint16 seq;
        WINDOW parent, wnd;
    };

    struct DestroyNotifyEvent
    {
        WINDOW wnd, eventWnd;
    };

    struct VisibilityNotifyEvent
    {
        WINDOW wnd;
        enum State {Unobscured = 0, PartiallyObscured, FullyObscured};
        byte state;
    };

};

#endif