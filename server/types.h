#ifndef _TYPES_H_
#define _TYPES_H_

#include <list>
#include <vector>
#include "SmartPointer.h"

namespace X
{
    typedef unsigned char *     STRING8;
    typedef __int32             int32;
    typedef unsigned __int32    uint32;
    typedef __int16             int16;
    typedef unsigned __int16    uint16;
    typedef unsigned char       byte;

    typedef uint32              WINDOW;
    typedef uint32              PIXMAP;
    typedef uint32              CURSOR;
    typedef uint32              FONT;
    typedef uint32              DRAWABLE;
    typedef uint32              GCONTEXT;
    typedef uint32              COLORMAP;
    typedef uint32              VISUALID;
    typedef uint32              VALUE;
    typedef uint32              TIMESTAMP;
    typedef uint32              BITMASK;
    typedef uint32              ATOM;
    typedef uint32              KEYSYM;
    typedef byte                KEYCODE;
    typedef byte                BUTTON;
    typedef std::vector<VALUE>  LISTofVALUE;
    typedef byte                BOOL;
    typedef uint32              PIXEL;
    typedef uint32              SETofEVENT;
    typedef uint32              SETofDEVICEEVENT;
    typedef uint32              SETofKEYMASK;
    typedef uint32              SETofPOINTEREVENT;
    typedef uint32              SETofKEYBUTMASK;
    typedef std::vector<KEYSYM> LISTofKEYSYM;
    class XWindow;
    class XPixmap;
    class XCursor;
    
    typedef XUtility::SmartPointer<XWindow> WindowPtr;
    typedef std::list<WindowPtr> WindowLst;
    typedef WindowLst::iterator WindowLstIt;

    typedef XUtility::SmartPointer<XPixmap> PixmapPtr;
    typedef XUtility::SmartPointer<XCursor> CursorPtr;
};
#endif