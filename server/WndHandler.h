#ifndef _WNDHANDLER_H_
#define _WNDHANDLER_H_

#include "IStream.h"
#include "ErrorStruct.h"

namespace X
{
    class XServer;
    class XClient;

    /* Class:           WndHandler
     * Description:     Handles window requests.
     */
    class WndHandler
    {
    public:
        WndHandler(XServer *, XClient *);

        // Create/modify/destroy

        // Creates a unmapped window
        int CreateUnmappedWindow(IStream * in, byte, ErrorStruct *);
        // changes the window attributes
        int ChangeWindowAttributes(IStream * in,ErrorStruct *);
        // Destroys a window.
        int DestroyWindow(IStream * in, ErrorStruct *);
        // Retrives the attributes
        int GetWindowAttributes(IStream * in, IStream * reply, uint32, ErrorStruct *);
        // configures the window
        int ConfigureWindow(IStream * in, ErrorStruct *);
        // destroys the subwindows.
        int DestroySubwindows(IStream * in, ErrorStruct *);

        // Map functions
        int MapWindow(IStream * in,ErrorStruct *);
        int UnmapWindow(IStream * in,ErrorStruct *);
        int MapSubwindows(IStream *, ErrorStruct *);
        int UnmapSubwindows(IStream *, ErrorStruct *);

    protected:
        XServer * m_server;
        XClient * m_client;
    };
};

#endif