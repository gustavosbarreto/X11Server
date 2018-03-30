#ifndef _XRENDER3D_H_
#define _XRENDER3D_H_

#include "IRenderer.h"
#include <glew.h>
#include <gl/gl.h>
#include <wglew.h>
#include <gl/glu.h>

namespace X
{
    /* Class:           XRender3d
     * Description:     
     */
    class XRender3d : public IRenderer
    {
    public:
        bool SetRenderTarget(IRenderTarget *);      // sets the current render target
        bool ReleaseRenderTarget();
        void Clear();                               // clears render target.
        void Fill(int, int, int);
        void Ortho2D(); 
        void RestrictDrawing(int, int, int, int);   // restricts the drawing.
        void RemoveRestriction();
    protected:
        IRenderTarget * m_renderTarget;
    };
};

#endif