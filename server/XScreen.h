#ifndef _XSCREEN_H_
#define _XSCREEN_H_

#include "ScreenType.h"
#include "XWindow.h"
#include "SmartPointer.h"
#include "types.h"
#include "IRenderSurface.h"

namespace X
{
    class XServer;
    /* Class:           XScreen
     * Description:     Represents a X screen
     */
    class XScreen
    {
    public:
        XScreen(XServer *, WINDOW,const DepthList &);
        const ScreenType & getScreenInfo() {return m_info;}
        bool isVisualValid(VISUALID, uint32) const;
        WINDOW GetRoot();
        XUtility::SmartPointer<XWindow> getRootWindow() {return m_root;}    // returns the root window.
        bool GetWindowAt(int , int, WindowPtr &);
        bool IsDepthSupported(byte);
        void SetRootWindow(WindowPtr);
        IRenderSurface * CreateRenderSurface(unsigned int, unsigned int);   // creates a offscreen surface.
    private:
        WindowPtr           m_root;     // pointer to the root window.
        WINDOW              m_rootId;       // the root window associated with the screen
        ScreenType          m_info;     // the screen information.
        const DepthList &   m_depths;   // the supported depths for this screen.
    };
};

#endif