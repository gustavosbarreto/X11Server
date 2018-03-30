#ifndef _PIXMAPIMPL_H_
#define _PIXMAPIMPL_H_

#include "XRectangle.h"
#include "SmartPointer.h"
#include "RefCounted.h"
#include "XDrawable.h"

namespace X
{
    /* Class:           PixmapImpl
     * Description:     Baseclass of the pixmap implementations.
     */
    class PixmapImpl : public XDrawable
    {
    public:
        XResource::ResourceType GetType() const {return XResource::PixmapResource;}

        // Tiles the area with the pixmap.
        virtual bool TileArea(const XRectangle &, XUtility::SmartPointer<PixmapImpl> pixmap) = 0;

        uint32 GetDepth() {return m_depth;}
        // fills a area of the pixmap with the supplied color.
        virtual void Fill(PIXEL, const X::XRectangle * rect = 0) = 0;
        virtual void PixelToComponents(X::PIXEL, int &, int &, int &) = 0;
        virtual bool PutPixmap(XUtility::SmartPointer<PixmapImpl> &, const X::XRectangle &) = 0;

    protected:
        int m_width, m_height, m_depth;
    };
};

#endif