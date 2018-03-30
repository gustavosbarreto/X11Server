#ifndef _SCREENINFO_H_
#define _SCREENINFO_H_

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include "XScreen.h"
#include "SmartPointer.h"
#include "XWindow.h"

namespace XWin32
{
    /* Function:        ScreenInfo
     * Description:     Contains information about the screen.
     */
    struct ScreenInfo
    {
        HWND                                hWnd;           // the window associated with the window.
        X::XScreen *                        screen;         // the screen.
        XUtility::SmartPointer<X::XWindow>  wnd;            // the root window.
    };
};

#endif