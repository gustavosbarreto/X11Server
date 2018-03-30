#include "XServer.h"
#include "common.h"
#include "ServerInfo.h"
#include <ctime>
#include <iostream>
#include "GLRenderer.h"
#include <assert.h>
#include "FBOSurface.h"

using namespace std;

namespace X
{
    /* Function:        XServer::XServer
     * Description:     Constructor, performs the required initalization.
     */
    XServer::XServer()
    {   
        m_keyboard      = NULL;
        m_pointer       = NULL;
        m_grabProxy     = NULL;
        m_grabClient    = NULL;
        m_activeScreen  = NULL;
    }

    /* Function:        XServer::~XServer
     * Description:     Destructor, performs the required cleanup.
     */
    XServer::~XServer()
    {
        delete m_keyboard;
        delete m_pointer;
        delete m_grabProxy;
    }

    /* Function:        XServer::CreateRenderSurface
     * Description:
     */
    IRenderSurface * XServer::CreateRenderSurface(unsigned int w, unsigned int h)
    {
        // use FBO:s for now
        FBOSurface * surface = new (std::nothrow) FBOSurface();
        if(surface == NULL) {
            cerr << "XServer::CreateRenderSurface: Failed to allocate the FBOSurface instance" << endl;
            return NULL;
        }
        if(!surface->Init(w, h)) {
            cerr << "XServer::CreateRenderSurface: Failed to initalize the FBOSurface" << endl;
            delete surface;
            return NULL;
        }   
        return surface;
    }

    /* Function:        XServer::ServerTime
     * Description:     Returns the current server time.
     */
    TIMESTAMP XServer::ServerTime()
    {
        return (TIMESTAMP) time(NULL);
    }

    /* Function:        XServer::InitServer
     * Description:     Initalizes the server
     */
    int XServer::InitServer(const char * config)
    {
        m_keyboard = new (std::nothrow) XKeyboard();
        if(m_keyboard == NULL) {
            cerr << "Could not allocate the keyboard instance" << endl;
            return -1;
        }
        if(!m_keyboard->LoadKeysyms("d:\\values.txt")) {
            cerr << "Failed to load the keysym definition file" << endl;
            return -1;
        }
        if(!m_keyboard->LoadKeymap("d:\\keymap.sv")) {
            cerr << "Failed to load the keymap" << endl;
            return -1;
        }

        if(!(m_pointer = new XPointer)) {
            cerr << "Failed to allocate the pointer instance" << endl;
            return -1;
        }

        // initalize the grab proxy
        m_grabProxy = new (std::nothrow) GrabProxy(this, m_keyboard, m_pointer);
        if(m_grabProxy == NULL) {
            cerr << "Failed to allocate the grabproxy" << endl;
            return -1;
        }

        if(CreatePixmapFromFile("d:\\bg.tga", m_bgPixmap) != 0) {
            cerr << "Failed to load the default background" << endl;
            return -1;
        }

        if(!InitScreens()) {
            cerr << "Failed to initalize the screens" << endl;
            return -1;
        }

        PIXMAP pId;
        


        // Initalize the pixmap formats
        /*FormatType bpp1, bpp8, bpp16, bpp32;
        
        // bitmask 1 bit per pixel
        bpp1.depth          = 1;
        bpp1.bitsPerPixel   = 1;
        bpp1.scanlinePadd   = 8;
        m_serverInfo.pixmapFormats.push_back(bpp1);

        // 8 bits per pixel
        bpp8.depth          = 8;
        bpp8.bitsPerPixel   = 8;
        bpp8.scanlinePadd   = 8;
        m_serverInfo.pixmapFormats.push_back(bpp8);

        // 16 bits per pixel
        bpp16.depth         = 16;
        bpp16.bitsPerPixel  = 16;
        bpp16.scanlinePadd  = 16;
        m_serverInfo.pixmapFormats.push_back(bpp16);

        // 32 bits per pixel
        bpp32.depth         = 24;
        bpp32.bitsPerPixel  = 32;
        bpp32.scanlinePadd  = 32;
        m_serverInfo.pixmapFormats.push_back(bpp32);

        */
        return 0;
    }

    /* Function:        XServer::GrabServer
     * Description:     The client attempts to grab the server
     */
    int XServer::GrabServer(const XClient * client, ErrorStruct * error)
    {
        if(m_grabClient != NULL) {
            // Should never happen
        } else {
            // the server is now grabbed by the client
            m_grabClient = client;
        }
        return 0;
    }

    /* Function:        XServer::UngrabServer
     * Description:     Ungrabs the server.
     */
    int XServer::UngrabServer(const XClient * client, ErrorStruct * error)
    {
        if(m_grabClient == NULL) {
            // The client can't ungrab the client, because the client hasen't grabbed the server
        } else {
            if(m_grabClient == client) {
                // Ungrab the server
                m_grabClient = NULL;        
            }
        }
        return 0;
    }

    /* Function:        XServer::ServerLoop
     * Description:     The actual server loop.
     */
    int XServer::ServerLoop()
    {
        if(m_grabClient) {
            // The server is grabbed, only process requests from this client
        } else {
            // The server is not grabbed, process requests from all the clients.
            CheckNewConnections();
            HandleClients();
        }
        return 0;
    }

    /* Function:        XServer::CheckNewConnections
     * Description:     Checks the connection for new clients.
     */
    int XServer::CheckNewConnections()
    {
        int ServerRequestSize = 30000;
        if(m_socket.isClientAvailable()) 
        {
            SocketLayer * socket = m_socket.AcceptClient();
            if(socket == NULL) {
                // Could not accept the connection
            } else {
                // The connection was accepted.
                XClient * client = new (std::nothrow) XClient(this, socket);
                if(client == NULL) {

                } 
                // add the client to the list with the other clients.
                m_clients.push_back(client);
            }
        }
        return 0;
    }

    /* Function:        XServer::HandleClients
     * Description:     Handles the clients.
     */
    int XServer::HandleClients()
    {
        /*
         * Let all the clients to some work.
         */
        for(std::list<XClient *>::iterator it = m_clients.begin();
            it != m_clients.end();
            it++)
        {
            // Do some work baby!
            int nret = (*it)->Process();
            switch(nret)
            {
            case REQUEST_AVAILABLE:
                // the client has a request, so handle it.
            case CLIENT_DISCONNECTED:
                // the client disconnected
            case CLIENT_AUTHENTICATION_FAILED:
                // could not authenticate the client.
            case FATAL_ERROR:
                // a fatal error occurred.
            case STATUS_SUCCESS:
                // nothing to report.
            default:
                // unknown reply.
                return 0;
            }
        }
        return 0;
    }

    /* Function:        XServer::GetServerInfo
     * Description:     Returns the server info.
     */
    const ServerInfo & XServer::getServerInfo()
    {
        return m_serverInfo;
    }

    /* Function:        XServer::DestroyResource
     * Description:     Destroys a resource.
     */
    void XServer::DestroyResource(DWORD id)
    {
        m_resources.Remove(id);
    }

    /* Function:        XServer::InitScreens
     * Description:     Initalizes the screens.
     */
    bool XServer::InitScreens()
    {
        WindowPtr parentPtr = 0;    // has no parent
        WindowPtr wnd = new (std::nothrow) XWindow(this, 1, parentPtr, 0);
        if(!wnd) {
            cerr << "Failed to allocate root window" << endl;
            return false;
        }
        DepthList depths;
        this->m_activeScreen = new XScreen(this, 1, depths); 
        CreateWindowStruct cs;
        cs.borderWidth      = 0;
        cs.h                = 512;
        cs.w                = 512;
        cs.parentId         = 0;
        cs.x                = 0;
        cs.y                = 0;
        cs.mask             = 0;
        cs.wId              = 1;

        // Now initalize the window
        if(wnd->Initalize(&cs,24, NULL, NULL) != 0) {
            cerr << "Failed to initalize the root window" << endl;
            return false;
        }
        // now set the background
        wnd->SetBackground(m_bgPixmap);
        // add the window
        m_resources.Insert(1, ResourcePtr(*wnd));
        // now set the root window for the screen.
        m_activeScreen->SetRootWindow(wnd);

        return true;
    }

    /*
     *
     */
    void XServer::render()
    {
        assert(m_activeScreen != NULL);
        WindowPtr root = m_activeScreen->getRootWindow();
        assert(root.valid());       
        root->DrawWindow(0, 0, root->getWidth(), root->getHeight());

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, g_testTextureID);
        GLRenderer::DrawTexturedRect(0,0, 100,100);
    }
};