#ifndef _IRENDERER_H_
#define _IRENDERER_H_

#include "IRenderTarget.h"
#include "SmartPointer.h"
#include "XWindow.h"

namespace X
{
    /* Class:           IRenderer
     * Description:     Interface class for the renderers.
     */
    class IRenderer
    {       
    public:
        // Sets the current render target
        virtual bool SetRenderTarget(IRenderTarget *) = 0;      // sets the current render target
        virtual bool ReleaseRenderTarget() = 0;
        virtual void Clear() = 0;                               // clears the render target.
        virtual void Fill(int, int, int) = 0;
        virtual void Ortho2D() = 0;                             // sets 2d mode.
        virtual bool RenderWindow(XUtility::SmartPointer<XWindow> &) = 0;
        virtual void RestrictDrawing(int, int, int, int) = 0;   // restricts the drawing.
        virtual void RemoveRestriction() = 0;

    };
};

#endif