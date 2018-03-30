#ifndef _WIN32SERVER_H_
#define _WIN32SERVER_H_

#include "XServer.h"
#include "types.h"
#include "XScreen.h"
#include "DepthType.h"
#include "ScreenInfo.h"
#include "GLWindow.h"

namespace XWin32
{
    /* Class:           Win32Server
     * Description:     Win32 implementation.
     */
    class Win32Server : public X::XServer
    {
    public:
        Win32Server();
        ~Win32Server();

        // starts the actual server.
        int Run();
        // Initalizes the server
        int Init(int, const char **);
        void Cleanup();
    protected:
        void render();
        void InitDepthList();
        X::DepthList                        m_depths;       // the supported depths for bitmaps and windows
        X::XScreen *                        m_screens[6];   // the screens.
        RenderX::GLWindow *                 m_wnd;          // the application window.
    };
};

#endif