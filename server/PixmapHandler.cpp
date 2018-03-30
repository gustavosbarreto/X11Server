#include "PixmapHandler.h"
#include "XServer.h"
#include "XScreen.h"
#include <assert.h>
#include <iostream>

using namespace std;

namespace X
{
    /* Function:        PixmapHandler::CreatePixmap 
     * Description:     Creates a pixmap.
     */
    int PixmapHandler::CreatePixmap(IStream * stream, byte depth, ErrorStruct * error)
    {
        PIXMAP pId;
        DRAWABLE dId;
        uint16 w, h;
        if(!stream->ReadUint32(pId) || 
            !stream->ReadUint32(dId) ||
            !stream->ReadUint16(w) ||
            !stream->ReadUint16(h))
        {
            cerr << "PixmapHandler::CreatePixmap: Failed to read the request from the stream" << endl;
            error->ErrorCode = ErrorStruct::LengthError;
            return -1;
        }
        // now check the dimensions
        if(w == 0 || h == 0)
        {
            cerr << "PixmapHandler::CreatePixmap: The width or height can't be zero" << endl;
            error->ErrorCode = ErrorStruct::ValueError;
            return -1;
        }
        // Now get the drawable
        DrawablePtr ptr;
        if(m_server->GetDrawable(dId, ptr, error) != 0)
            return -1;
        
        // is the depth supported?
        XScreen * screen = ptr->GetScreen();
        assert(screen != NULL);
        if(!screen->IsDepthSupported(depth))
        {
            cerr << "PixmapHandler::CreatePixmap: The specified depth is not supported by the screen" << endl;
            error->ErrorCode = ErrorStruct::ValueError;
            return -1;
        }
        // now create the actual pixmap
        if(m_server->CreatePixmap(pId, depth, w, h, ptr->GetScreen(), error) != 0)
            return -1;
        // return success.
        return 0;
    }

    /* Function:        PixmapHandler::FreePixmap
     * Description:     Frees a pixmap.
     */
    int PixmapHandler::FreePixmap(IStream * stream, ErrorStruct * error)
    {
        PIXMAP pId;
        if(!stream->ReadUint32(pId))
        {
            cerr << "PixmapHandler::FreePixmap: Failed to read the request" << endl;
            error->ErrorCode = ErrorStruct::LengthError;
            return -1;
        }
        if(m_server->FreePixmap(pId, error) != 0)
            return -1;
        return 0;
    }
};