#ifndef _PBUFFER_H_
#define _PBUFFER_H_

#include <glew.h>
#include <gl/gl.h>
#include <wglew.h>

namespace X
{
    /* Struct:          PBuffer
     * Description:     A p-buffer used for off-screen rendering.
     */
    struct PBuffer
    {
        HDC             m_hDC;          // the device context   
        HGLRC           m_hRC;          // the rendering context
        HPBUFFERARB     m_hPBuffer;     // the handle to the p-buffer
        int             m_iWidth,       // the width of the buffer
                        m_iHeight;      // the height of the buffer.
        unsigned int    m_textureId;
    };
};


#endif