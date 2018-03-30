#ifndef _XDRAWABLE_H_
#define _XDRAWABLE_H_

#include "XResource.h"
#include "XGContext.h"
#include "types.h"
#include "PBuffer.h"
#include "IRenderTarget.h"
#include "IRenderSurface.h"

namespace X
{
    class XScreen;
    class XServer;

    /* Class:           XDrawable
     * Description:
     */
    class XDrawable : public XResource , public IRenderTarget
    {
    public:
        XDrawable(uint32, XScreen *, XServer *);
        ~XDrawable();

        virtual bool isDrawable() {return true;}

        IRenderSurface * GetSurface() {return m_surface;}
        bool BindTarget();
        // draws a bitmap image.
        int PutBitmapImage(const XGContext *, int,int, byte, int, int, byte, byte * src, size_t srclen);
        int PutZImage(const XGContext *, int,int,int,int,const byte *, size_t);
        int GetZImage(int,int,int,int, BITMASK,byte *, size_t & len);       
        XRectangle GetRect() const {return m_rect;}
        XScreen * GetScreen()                   {return m_screen;}
        int GetDepth() {return m_depth;}
    protected:
        bool AllocSurface(unsigned int, unsigned int);      // allocates the surface.

        XRectangle          m_rect;
        WINDOW              m_root;                     
        int                 m_depth;
        IRenderSurface *    m_surface;
        XScreen *           m_screen;
        XServer *           m_server;
    };

    typedef XUtility::SmartPointer<XDrawable> DrawablePtr;
};

#endif