#include "WndHandler.h"
#include "XServer.h"
#include "RequestStructs.h"
#include <iostream>
#include "common.h"
#include "XClient.h"

using namespace std;

namespace X
{
    /* Function:        NumBits
     * Description:     Counts the number of bits in the bitmask.
     */
    size_t CountBits(BITMASK mask)
    {
        size_t num = 0;
        while(mask) {
            ++num;
            mask >>= 1;
        }
        return num;
    }
    /* Funcion:         WndHandler::CreateUnmappedWindow
     * Description:     Creates a unmapped window.
     * Created:         2008-01-18 (maglek)
     */
    int WndHandler::CreateUnmappedWindow(IStream * in, byte depth, ErrorStruct * error)
    {
        // Read the information.
        CreateWindowStruct cs;
        error->major = 0;
        // TODO

        // First read the request data from the stream.
        if(!in->ReadUint32(cs.wId) || !in->ReadUint32(cs.parentId) ||
            !in->ReadInt16(cs.x) || !in->ReadInt16(cs.y) ||
            !in->ReadUint16(cs.w) || !in->ReadUint16(cs.h) ||
            !in->ReadUint16(cs.borderWidth) || !in->ReadUint16(cs.windowClass) ||
            !in->ReadUint32(cs.mask))
        {
            cerr << "WndHandler::CreateUnmappedWindow: Failed to read the request from the stream" << endl;
            // A length error
            error->ErrorCode = ErrorStruct::LengthError;
        }

        // now read the attributes
        size_t bits = CountBits(cs.mask);
        for(size_t i = 0;i<bits;i++) {
            VALUE val;
            if(!in->ReadUint32(val)) {
                cerr << "WndHandler::CreateUnmappedWindow: Failed to read the request from the stream" << endl;
                error->ErrorCode = ErrorStruct::LengthError;
            }
            cs.values.push_back(val);
        }

        WindowPtr ptrWnd, ptrParent;
        if(!m_server->XFindWindow(cs.parentId, ptrParent, error)) {
            cerr << "WndHandler::CreateUnmappedWindow: The parent window does not exist" << endl;
            return -1;
        }

        // Is the resource id free?
        if(!m_server->isFree(cs.wId)) {
            // The resource id is not free.
            error->ErrorCode = ErrorStruct::IDChoiceError;
            return -1;
        }

        // Create the window
        ptrWnd = new (std::nothrow) XWindow(m_server, cs.wId, ptrParent, ptrParent->GetScreen());
        if(!ptrWnd) {
            cerr << "CreateUnmappedWindow: Failed to allocate the window" << endl;
            error->ErrorCode = ErrorStruct::AllocError;
            return -1;
        }

        // now initalize the window.
        if(!ptrWnd->Initalize(&cs, depth, m_client, error)) {
            // could not initalize the window.
            cerr << "WndHandler::CreateUnmappedWindow: Failed to initalize the window." << endl;
            return -1;
        }
        // Success.
        return 0;
    }

    /* Function:        WndHandler::MapWindow
     * Description:     Maps a window.
     */
    int WndHandler::MapWindow(IStream * in, ErrorStruct * error)
    {
        // read the window id.
        WINDOW wId;
        if(!in->ReadUint32(wId)) {
            cerr << "WndHandler::MapWindow: Failed to read the request" << endl;
            error->ErrorCode = ErrorStruct::LengthError;
            return -1;
        }
        // now find the window
        WindowPtr wnd;
        if(!m_server->XFindWindow(wId, wnd, error)) {
            // The window does not exist
            return -1;
        }

        // now map the window
        wnd->MapWindow(m_client);
        return 0;
    }

    /* Function:        WndHandler::UnmapWindow
     * Description:     Unmaps a window.
     */
    int WndHandler::UnmapWindow(IStream * in, ErrorStruct * error)
    {
        // read the window id.
        WINDOW wId;
        if(!in->ReadUint32(wId)) {
            cerr << "WndHandler::MapWindow: Failed to read the request" << endl;
            error->ErrorCode = ErrorStruct::LengthError;
            return -1;
        }
        // now find the window
        WindowPtr wnd;
        if(!m_server->XFindWindow(wId, wnd, error)) {
            // The window does not exist
            return -1;
        }
        // now map the window
        wnd->UnmapWindow(false);
        return 0;
    }

    /* Function:        WndHandler::DestroyWindow
     * Description:     Destroys a window.
     */
    int WndHandler::DestroyWindow(IStream * in, ErrorStruct * error)
    {
        WINDOW wId;
        if(!in->ReadUint32(wId)) {
            cerr << "WndHandler::DestroyWindow: the request was too short" << endl;
            error->ErrorCode = ErrorStruct::LengthError;
            return -1;
        }
        // now let the server destroy the window
        if(!m_server->XDestroyWindow(wId, error)) {
            cerr << "WndHandler::DestroyWindow: Could not destroy the requested window" << endl;
            return -1;
        }
        // the window was destroyed.
        return 0;
    }

    /* Function:        WndHandler::ChangeWindowAttributes
     * Description:     Changes the window attributes.
     */
    int WndHandler::ChangeWindowAttributes(IStream * in, ErrorStruct * error)
    {
        WINDOW wId;
        BITMASK bitmask;
        LISTofVALUE values;
        WindowPtr ptrWnd;

        if(!in->ReadUint32(wId) || !in->ReadUint32(bitmask)) {
            cerr << "ChangeWindowAttributes: Request too short" << endl;
            error->ErrorCode = ErrorStruct::LengthError;
            return -1;
        }
        size_t bits = CountBits(bitmask);
        // now read the values.
        for(size_t i = 0; i < bits;++i) {
            VALUE value;
            if(!in->ReadUint32(value)) {
                cerr << "ChangeWindowAttributes: Request too short" << endl;
                error->ErrorCode = ErrorStruct::LengthError;
                return -1;
            }
        }
        // Now get the window
        if(!m_server->XFindWindow(wId, ptrWnd, error)) {
            cerr << "ChangeWindowAttributes: The Window identifier is not valid" << endl;
            return -1;
        }
        // now change the actual attributes
        if(!ptrWnd->ChangeWindowAttributes(bitmask, values, m_client, error)) {
            return -1;
        }
        // success.
        return 0;
    }

    /* Function:        WndHandlder::GetWindowAttributes
     * Description:     Retrives the attributes of the window.
     */
    int WndHandler::GetWindowAttributes(IStream * in, IStream * reply,uint32 seq, ErrorStruct * error)
    {
        WINDOW wId;
        WindowPtr ptrWnd;
        if(!in->ReadUint32(wId)) {
            cerr << "GetWindowAttributes: The request is too short" << endl;
            error->ErrorCode = ErrorStruct::LengthError;
            return -1;
        }
        // now get the window, if it exist
        if(!m_server->XFindWindow(wId, ptrWnd, error)) {
            cerr << "GetWindowAttributes: The specified window does not exist" << endl;
            error->ErrorCode = ErrorStruct::WindowError;
            return -1;
        }
        AttributeStruct attrs;
        // now retrive the attributes
        ptrWnd->GetAttributes(&attrs, m_client);
        // now write the data to the reply stream
        
        if(!reply->WriteByte(1) ||                          // 1 = reply
            !reply->WriteByte(attrs.BackingStore) ||
            !reply->WriteUint16(seq) ||
            !reply->WriteUint32(3) ||
            !reply->WriteUint32(attrs.visual) ||
            !reply->WriteUint16(attrs.WindowClass) ||
            !reply->WriteByte(attrs.BitGravity) ||
            !reply->WriteByte(attrs.WinGravity) ||
            !reply->WriteUint32(attrs.BackingPlanes) ||
            !reply->WriteUint32(attrs.BackingPixel) ||
            !reply->WriteByte(attrs.SaveUnder) ||
            !reply->WriteByte(attrs.MapIsInstalled) ||
            !reply->WriteByte(attrs.MapState) ||
            !reply->WriteByte(attrs.OverrideRedirect) ||
            !reply->WriteUint32(attrs.ColorMap) ||
            !reply->WriteUint32(attrs.AllEventMasks) ||
            !reply->WriteUint32(attrs.yourEventMasks) ||
            !reply->WriteInt32(attrs.doNotPropagateMask) ||
            !reply->WriteInt16(0))                          // padd the data to a multiple of 4
        {
            cerr << "GetWindowAttributes: Failed to write the request to the stream" << endl;
            return -1;
        }
        // success.
        return 0;
    }

    /* Function:        WndHandler::ConfigureWindow
     * Description:     Configures the window.
     */
    int WndHandler::ConfigureWindow(X::IStream *in, ErrorStruct * error)
    {
        WINDOW wId;
        BITMASK mask;
        LISTofVALUE values;
        if(!in->ReadUint32(wId) || !in->ReadUint32(mask)) {
            cerr << "ConfigureWindow: Requets too short" << endl;
            error->ErrorCode = ErrorStruct::LengthError;
            return -1;
        }
        if(mask & 0x7F) {
            // a invalid flag is set
            cerr << "ConfigureWindow: Invalid bitmask supplied" << endl;
            error->ErrorCode = ErrorStruct::ValueError;
            return -1;
        }
        size_t bits = CountBits(mask);
        for(size_t i = 0;i<bits;++i) {
            VALUE val;
            if(!in->ReadUint32(val)) {
                cerr << "ConfigureWindow: Requets too short" << endl;
                error->ErrorCode = ErrorStruct::LengthError;
                return -1;
            }
            values.push_back(val); // add the value.
        }
        // now get the window
        WindowPtr wndPtr;
        if(!m_server->XFindWindow(wId, wndPtr, error)) {
            cerr << "ConfigureWindow: the supplied window id does not represent a window." << endl;
            error->ErrorCode = ErrorStruct::WindowError;
            return -1;
        }
        // now configure the window.
        if(!wndPtr->ConfigureWindow(mask, values, error)) {
            return -1;
        }
        return 0;
    }

    /* Function:        WndHandler::DestroySubwindows
     * Description:     Destroys the subwindows.
     */
    int WndHandler::DestroySubwindows(IStream * in, ErrorStruct * error)
    {
        WINDOW wId;
        if(!in->ReadUint32(wId)) {
            cerr << "DestroySubwindows: The request is too short" << endl;
            error->ErrorCode = ErrorStruct::LengthError;
            return -1;
        }
        // get the winow
        WindowPtr wndPtr;
        if(!m_server->XFindWindow(wId, wndPtr, error)) {
            cerr << "DestroySubwindows: The supplied id does not name a existing window" << endl;
            error->ErrorCode = ErrorStruct::WindowError;
            return -1;
        }
        // destroy the subwindows.
        wndPtr->DestroySubWindows();
        // success
        return 0;
    }
    
    /* Function:        WndHandler::MapSubwindows
     * Description:     Maps the subwindows.
     */
    int WndHandler::MapSubwindows(IStream * in, ErrorStruct * error)
    {
        WINDOW wId;
        WindowPtr wndPtr;
        if(!in->ReadUint32(wId)) {
            cerr << "MapSubwindows: The request is too short" << endl;
            error->ErrorCode = ErrorStruct::LengthError;
            return -1;
        }
        // now find the window
        if(!m_server->XFindWindow(wId, wndPtr, error)) {
            cerr << "MapSubwindows: The id does not identify a existing window" << endl;
            error->ErrorCode = ErrorStruct::WindowError;
            return -1;
        }
        wndPtr->MapSubWindows(m_client);
        return 0;
    }

    /*
     *
     */
    int WndHandler::UnmapSubwindows(IStream * in, ErrorStruct * error)
    {
        WINDOW wId;
        WindowPtr wndPtr;
        if(!in->ReadUint32(wId)) {
            cerr << "UnmapSubwindows: The request is too short" << endl;
            error->ErrorCode = ErrorStruct::LengthError;
            return -1;
        }
        // now find the window
        if(!m_server->XFindWindow(wId, wndPtr, error)) {
            cerr << "UnmapSubwindows: The id does not identify a existing window" << endl;
            error->ErrorCode = ErrorStruct::WindowError;
            return -1;
        }
        wndPtr->UnmapSubWindows();
        return 0;
    }
};