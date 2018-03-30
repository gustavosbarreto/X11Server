#ifndef _XSERVER_H_
#define _XSERVER_H_

#include "XClient.h"
#include "BinaryTree.h"
#include "XResource.h"
#include "XScreen.h"
#include "ServerInfo.h"
#include "types.h"
#include "XWindow.h"
#include "SmartPointer.h"
#include "AtomList.h"
#include "ErrorStruct.h"
#include "AttributeStruct.h"
#include "PixelFormat.h"
#include "IInputProxy.h"
#include "GrabProxy.h"
#include <list>
#include "PointerData.h"
#include "KeyboardData.h"
#include "IRenderer.h"
#include "IRenderSurface.h"

class XClient;

namespace X
{
    /* Class:           XServer
     * Description:     The actual X server.
     */
    class XServer : public IInputProxy
    {
    public:
        XServer();
        ~XServer();

        const ServerInfo & getServerInfo(); 

        int InitServer(const char *);
        /*
         *
         */
        void MouseMoved(int x,int y, unsigned int mask);
        void ButtonPressed(int x,int y, BUTTON button, unsigned int mask);

        /* 
         * Window functions
         */
        int XCreateWindow(X::WINDOW wId, WindowPtr & wnd, WindowPtr &);
        int XDestroyWindow(WINDOW, ErrorStruct *);
        bool XFindWindow(WINDOW, WindowPtr &, ErrorStruct *);

        /*
         * Pixmap functions
         */
        int CreatePixmap(PIXMAP, byte, int , int, XScreen *, ErrorStruct *);    // Creates a pixmap with the supplied id.
        int FindPixmap(PIXMAP, PixmapPtr &, ErrorStruct *); // retrives the pixmap with the supplied it.
        int FreePixmap(PIXMAP, ErrorStruct *); // frees the pixmap, won't actually delete it.
        int CreatePixmapFromFile(const char *, PixmapPtr &); // creates a pixmap from a file

        /*
         * Atom functions.
         */
        bool XGetAtomName(ATOM atom, std::wstring & name);
        bool XInternAtom(const std::wstring & name, ATOM & atom, bool onlyIfExist);


        int GrabServer(const XClient * client, ErrorStruct * error);
        int UngrabServer(const XClient * client, ErrorStruct * error);

        TIMESTAMP ServerTime();
        void DestroyResource(DWORD);

        bool ConfinePointerTo(WindowPtr);
        bool GetResource(uint32, ResourcePtr &);
        void RemoveResource(uint32);
        bool isFree(uint32);
        int GetDrawable(uint32, DrawablePtr &, ErrorStruct *);

        IRenderSurface * CreateRenderSurface(unsigned int, unsigned int);
    protected:
        bool InitScreens();
        virtual void render();
        int ServerLoop();               // the actual server loop.
        int CheckNewConnections();      
        int HandleClients();

        bool GetWindowResource(WINDOW, WindowPtr &, ErrorStruct *);
        int SetClientNotify(BITMASK, XClient *);
 
        SocketLayer                                                     m_socket;
        std::list<XClient *>                                            m_clients;      // the connected clients.
        XUtility::BinaryTree<uint32, XUtility::SmartPointer<XResource> >    m_resources;    // the resources.
        std::list<XScreen *>                                            m_screens;      // the screens.
    
        const XClient *                                                 m_grabClient;   // the client that has grabbed the server, if any.
        AtomList                                                        m_atoms;        // the stored atoms.
        ServerInfo                                                      m_serverInfo;
        // the current screen
        XScreen *                                                       m_activeScreen; 
        GrabProxy *                                                     m_grabProxy;
        std::list<PointerData>                                          m_motionBuffer;
        XKeyboard *                                                     m_keyboard;
        IRenderer *                                                     m_renderer;
        XPointer *                                                      m_pointer;
        PixmapPtr                                                       m_bgPixmap;
        unsigned int g_testTextureID;
    };
};

#endif