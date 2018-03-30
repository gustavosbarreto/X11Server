#ifndef _PIXMAPHANDLER_H_
#define _PIXMAPHANDLER_H_

#include "IStream.h"
#include "ErrorStruct.h"

namespace X
{
    class XServer;

    /* Class:           PixmapHandler
     * Description:     Implements the pixmap requests.
     */
    class PixmapHandler
    {
    public:
        PixmapHandler(XServer *);       // constructor
        ~PixmapHandler();

        int CreatePixmap(IStream *,byte, ErrorStruct *);
        int FreePixmap(IStream *, ErrorStruct *);
    protected:
        XServer * m_server;
    };
};

#endif