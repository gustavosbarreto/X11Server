#ifndef _XPIXMAP_H_
#define _XPIXMAP_H_

#include "XDrawable.h"
#include "PixelFormat.h"
#include "SmartPointer.h"

namespace X
{
    /* Class:           XPixmap
     * Description:     A offscreen rendering buffer. Stored in Z format.
     */
    class XPixmap : public XDrawable
    {
    public:
        XPixmap(PIXMAP, XScreen *, XServer *);
        ~XPixmap();
        
        ResourceType GetType() const {return XResource::PixmapResource;}
        int Init(unsigned int w, unsigned int h, byte depth);
    };

    typedef XUtility::SmartPointer<XPixmap> PixmapPtr;
};

#endif