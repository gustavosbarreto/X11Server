#ifndef _XCURSOR_H_
#define _XCURSOR_H_

#include "SmartPointer.h"
#include "XResource.h"
#include "types.h"

namespace X
{
    /* Class:           XCursor
     * Description:     Represents a cursor.
     */
    class XCursor : public XResource
    {
    public:
        XCursor(XUtility::SmartPointer<XPixmap> &,      // the source pixmap
            XUtility::SmartPointer<XPixmap> &,          // the mask pixmap
            XPoint<int16> & hotspot,                            // the hotspot
            PIXEL fg,                                   // the foreground color
            PIXEL bg);                                  // the background color.
        
        ~XCursor();

        
        void ChangeColor(PIXEL, PIXEL);
    protected:
        PixmapPtr m_source, m_mask; 
        PIXEL m_fg,                                 // the foreground color 
            m_bg;                                   // the background color.
    };
};

#endif