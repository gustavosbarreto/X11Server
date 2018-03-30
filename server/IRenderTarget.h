#ifndef _IRENDERTARGET_H_
#define _IRENDERTARGET_H_

namespace X
{
    /* Class:           IRenderTarget
     * Description:     Baseclass for the render targets.
     */
    class IRenderTarget
    {
    public:
        virtual int getWidth() {return m_width;}
        virtual int getHeight() {return m_height;}
        virtual bool BindTarget() = 0;
        virtual void SwapBuffers() {}
    protected:
        int m_width, m_height;
    };
};

#endif