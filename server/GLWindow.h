#ifndef _GLWINDOW_H_
#define _GLWINDOW_H_

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <glew.h>
#include <gl/gl.h>
#include <wglew.h>
#include "IRenderTarget.h"
#include "IInputProxy.h"

namespace RenderX
{
    /* Function:            GLWindow
     * Description:         
     */
    class GLWindow : public X::IRenderTarget
    {
    public:
        GLWindow(X::IInputProxy *);
        ~GLWindow();

        // Creates the window.
        int Create(int w, int h);
        // Shows or hides the window.
        void Show(bool visible = true); 
        
        bool BindTarget();
        int getWidth() {return m_width;}
        int getHeight() {return m_height;}
        void SwapBuffers();

        HDC GetDeviceContext() {return m_hDC;}
        static LRESULT CALLBACK WndProc(HWND, unsigned int, WPARAM, LPARAM);
    private:

        /* 
         * Private functions
         */
        int InitGL();       // initalizes OpenGL support for the window.

        /*
         * Private data.
         */
        HWND                m_hWnd;         // the window handle
        HDC                 m_hDC;          // the device context
        HGLRC               m_hRC;          // the rendering context.   
        X::IInputProxy *    m_inputProxy;   // input proxy, delegates the input events.
    };
};


#endif