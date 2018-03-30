#include "Win32Server.h"
#include <iostream>
#include "glew.h"
#include <gl/gl.h>
#include "wglew.h"
#include "GLRenderer.h"

namespace XWin32
{
    using namespace std;
    using namespace X;
    using namespace XUtility;

    /*
     *
     */
    Win32Server::Win32Server()
    {
    }

    /*
     *
     */
    Win32Server::~Win32Server()
    {
    }

    /* Function:        Win32Server::InitDepthList
     * Description:     Initalizes the list with the supported depths.
     */
    void Win32Server::InitDepthList()
    {
        // initalize the supported depths
        DepthType depth1, depth8, depth16, depth24;
        VisualType type16, type24;
        
        // 1 bit (Pixmap only)
        depth1.depth = 1;

        // 8 bit (Pixmap only)
        depth8.depth = 8;

        // 16 bit 
        depth16.depth                   = 16;
        type16.id                       = 1;
        type16.type_class               = VisualType::TrueColor;
        type16.bits_per_rgb_value       = 16;
        type16.colourmap_entries        = 65536; // 2^16
        type16.red_mask                 = 0x1F;
        type16.green_mask               = 0x1F << 5;
        type16.blue_mask                = 0x1F << 10;
        depth16.visuals.push_back(type16);

        // 24 bit
        depth24.depth                   = 24;
        type24.id                       = 2;
        type24.type_class               = VisualType::TrueColor;
        type24.bits_per_rgb_value       = 32;
        type24.colourmap_entries        = 16777216; // 2^24
        type24.red_mask                 = 0xFF;
        type24.green_mask               = 0xFF00;
        type24.blue_mask                = 0xFF0000;
        depth24.visuals.push_back(type24);

        // 
        m_depths.push_back(depth1);
        m_depths.push_back(depth8);
        m_depths.push_back(depth16);
        m_depths.push_back(depth24);
    }


    /* Function:        Win32Server::Init
     * Description:     Initalizes the server
     */
    int Win32Server::Init(int argc , const char ** argv)
    {
        
        // Create the primary window
        m_wnd = new (std::nothrow) RenderX::GLWindow(this);
        if(m_wnd == NULL) {
            MessageBox(0, L"Could not create the primary window", L"Fatal Error", MB_ICONEXCLAMATION);
            return -1;
        }
        if(m_wnd->Create(800, 600) != 0) {
            MessageBox(0, L"Could not initalize the primary window", L"Fatal Error", MB_ICONEXCLAMATION);
            return -1;
        }

        // initalize the list with the depths
        InitDepthList();
        // initalize the server
        if(InitServer(NULL) != 0) {
            cerr << "Win32Server::Init: InitServer() failed" << endl;
            return -1;
        }
        return 0;
    }

    /*
     *
     */
    int Win32Server::Run()
    {
        m_wnd->Show(true);
        MSG msg;
        while(true)
        {
            if(PeekMessage(&msg, 0,0,0, PM_REMOVE)) {
                if(msg.message == WM_QUIT) {
                    return 0;
                }
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            // render the active screen
            render();
            Sleep(1);
        }
        return 0;
    }

    /*
     *
     */
    void Win32Server::Cleanup()
    {
        if(m_wnd) {
            delete m_wnd;
            m_wnd = NULL;
        }
    }

    void Win32Server::render()
    {
        // bind the window as the target
        m_wnd->BindTarget();
        // now perform the actual rendering.
        GLRenderer::Ortho2d(0, m_wnd->getWidth(), 0, m_wnd->getHeight());
        GLRenderer::LoadIdentity();
        XServer::render();
        // swap the buffers.
        GLRenderer::SwapBuffers(m_wnd);
    }

};