#ifndef _IRENDERSURFACE_H_
#define _IRENDERSURFACE_H_

#include <glew.h>
#include <gl/gl.h>
#include <wglew.h>

#include "XRectangle.h"

namespace X
{
    /* Function:        IRenderSurface
     * Description:     Interface class for a render surface.
     */
    class IRenderSurface
    {
    public:
        virtual bool Init(unsigned int, unsigned int) = 0;
        virtual ~IRenderSurface() {}
        virtual bool BindTarget()       = 0;
        virtual bool UnbindTarget()     = 0;
        virtual bool BindTexture()      = 0;
        virtual bool UnbindTexture()    = 0;
        void GetDimensions(unsigned int & w, unsigned int & h) {w = m_width; h = m_height;}
    protected:
        unsigned int m_width, m_height;
    };
};

#endif