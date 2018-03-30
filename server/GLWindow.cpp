#include "GLWindow.h"
#include <iostream>

namespace RenderX
{
    using namespace std;
    using namespace X;

    /*
     *
     */
    GLWindow::GLWindow(X::IInputProxy * proxy)
    {
        m_inputProxy = proxy;
        m_hDC = 0;
        m_hRC = 0;
    }

    /*
     *
     */
    GLWindow::~GLWindow()
    {
        wglMakeCurrent(0,0);
        if(m_hDC) DeleteDC(m_hDC);
        if(m_hRC) wglDeleteContext(m_hRC);
    }

    /* Function:        GLWindow::Create
     * Description:     Creates the actual window.
     */
    int GLWindow::Create(int w, int h)
    {
        m_width = w;
        m_height = h;

        // Register the window class.
        WNDCLASS wc;
        if(!GetClassInfo(GetModuleHandle(0), L"XServer", &wc)) {
            ZeroMemory(&wc, sizeof(wc));
            wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
            wc.hInstance = GetModuleHandle(0);
            wc.lpfnWndProc = GLWindow::WndProc;
            wc.lpszClassName = L"XServer";
            wc.style =  CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
            if(!RegisterClass(&wc)) {
                MessageBoxW(0, L"Could not register the window class", L"Fatal Error", MB_ICONEXCLAMATION);
                return -1;
            }
        }
        // now create the window
        m_hWnd = CreateWindowW(L"XServer", L"X Server",WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,
            CW_USEDEFAULT,m_width, m_height, 0, 0, GetModuleHandle(0), this);
        if(m_hWnd == 0) {
            MessageBoxW(0, L"Could not create the window", L"Fatal Error", MB_ICONEXCLAMATION);
            return -1;
        }
        // Now initalize OpenGL.
        return InitGL();
    }

    /* Function:        GLWindow::InitGL
     * Description:     Initalizes OpenGL support for the window.
     */
    int GLWindow::InitGL()
    {
        // First initalize OpenGL
        PIXELFORMATDESCRIPTOR pfd;
        memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

        pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.nVersion   = 1;
        pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 16;
        pfd.cDepthBits = 16;

        m_hDC = GetDC(m_hWnd);
        int PixelFormat = ChoosePixelFormat(m_hDC, &pfd);
        SetPixelFormat(m_hDC, PixelFormat, &pfd);
        m_hRC = wglCreateContext(m_hDC);
        wglMakeCurrent(m_hDC, m_hRC);

        // Then initalize glew.
        if(glewInit() != GLEW_OK) {
            MessageBox(m_hWnd, L"Failed to initalize GLEW", L"Fatal Error", MB_OK);
            return -1;
        }
        return 0;
    }

    /* Function:        GLWindow::bind
     *
     */
    bool GLWindow::BindTarget()
    {
        if(!wglMakeCurrent(m_hDC, m_hRC)) {
            cerr << "wglMakeCurrnt() failed for window" << endl;
            return false;
        }
        glViewport(0,0, m_width, m_height);
        return true;
    }

    /* Function:        GLWindow::WndProc
     * Description:     Handles the window messages.
     */
    LRESULT CALLBACK GLWindow::WndProc(HWND hWnd, unsigned int msg, WPARAM wParam, LPARAM lParam)
    {
        static WINDOWINFO info;
        switch(msg)
        {
        case WM_MOUSEMOVE:
            {
                int x = int(lParam & 0xFF);
                int y = int(lParam >> 16);
                GLWindow * wnd = (GLWindow *) GetWindowLongPtr(hWnd, GWLP_USERDATA);
                // now notify the input proxy about the event.
                wnd->m_inputProxy->MouseMoved(x, y, 0);
                return 0;
            }
        case WM_CREATE:
            {
                // Save the pointer to the window instance.
                CREATESTRUCT * cs = (CREATESTRUCT *) lParam;
                GLWindow * instance = (GLWindow *) cs->lpCreateParams;
                if(!SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR) instance) && (GetLastError() != 0)) {
                    cerr << "SetWindowLongPtr() failed" << endl;
                    return -1;
                }
                return 0;
            }
        case WM_CLOSE:
        case WM_DESTROY:
            cerr << "Destroying window" << endl;
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
        }
    }

    /*
     *
     */
    void GLWindow::Show(bool visible)
    {
        if(visible)
            ShowWindow(m_hWnd, SW_SHOW);
        else
            ShowWindow(m_hWnd, SW_HIDE);
    }

    /*
     *
     */
    void GLWindow::SwapBuffers()
    {
        ::SwapBuffers(m_hDC);
    }
};