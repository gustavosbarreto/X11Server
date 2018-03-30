#include "XRender3d.h"

namespace X
{
    /* Function:        XRender3d::SetRenderTarget
     * Description:     Sets the render target.
     */
    bool XRender3d::SetRenderTarget(IRenderTarget * target)
    {
        // Set the render target and bind it.
        m_renderTarget = target;
        return target->bindAsTarget();
    }

    /* Function:        XRender3d::ReleaseRenderTarget  
     * Description:     Releases and unbinds the current render target.
     */
    bool XRender3d::ReleaseRenderTarget()
    {
        if(m_renderTarget) {
            m_renderTarget->unbindTarget();
            m_renderTarget = NULL;
            return true;
        }
        return false;
    }

    /* Function:        XRender3d::Clear
     * Description:     Clears the render target.
     */
    void XRender3d::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    /* Function:        XRender3d::RestrictDrawing
     * Description:     Restricts the drawing to inside the rectangle.
     */
    void XRender3d::RestrictDrawing(int x, int y, int w, int h)
    {
        glEnable(GL_SCISSOR_TEST);
        glScissor(x, y, w, h);
    }

    /* Function:        XRender3d::RemoveRestriction
     * Description:     Removes the current clipping restrictions.
     */
    void XRender3d::RemoveRestriction()
    {
        glDisable(GL_SCISSOR_TEST);
    }

    /*
     *
     */
    void XRender3d::Ortho2D()
    {
        if(!m_renderTarget) 
            return;
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0,m_renderTarget->getWidth(), 0, m_renderTarget->getHeight());
        glMatrixMode(GL_MODELVIEW);
    }
};