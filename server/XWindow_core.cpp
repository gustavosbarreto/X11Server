#include "XWindow.h"
#include <iostream>
#include "XDefines.h"
#include "XServer.h"
#include <assert.h>

#define InputOnlyClass          2
#define InputOutputClass        1
#define CopyFromParentClass     0

namespace X
{
    using namespace std;
    /* Function:        XWindow::XWindow
     * Description:     Constructor
     */
    XWindow::XWindow(XServer * server,      // the x server instance
        WINDOW wId,                         // the window id    
        WindowPtr & parent,                 // the parent window.
        XScreen * screen)
        : XDrawable(wId, screen, server), 
        m_mapType(WhenMapped),              // default to this
        m_mapped(false)                     // the window is created unmapped.
    {

    }

    XWindow::~XWindow()
    {
        cerr << "XWindow::~XWindow" << endl;
    }

    /* Function:        XWindow::Initalize
     * Description:     Initalizes the window.
     */
    int XWindow::Initalize(CreateWindowStruct * cws, byte depth, XClient * client, ErrorStruct * error)
    {
        if(client != NULL && cws->wId == 0) {
            cerr << "XWindow::Initalize: Invalid window identifier (0) supplied" << endl;
            return -1;
        }
        m_rect.m_x = cws->x;
        m_rect.m_y = cws->y;
        if(cws->w == 0 || cws->h == 0) {
            cerr << "XWindow::Initalize: Invalid dimensions of the window" << endl;
            error->ErrorCode = ErrorStruct::ValueError;
            return -1;
        }
        m_rect.m_height     = cws->h;
        m_rect.m_width      = cws->w;

        WindowPtr parent;
        XScreen * screen = 0;
        if(cws->parentId != 0)
        {
            if(m_server->XFindWindow(cws->parentId, parent, error)) {
                if(client != NULL) {
                    return false;
                }
            }
            screen = parent->GetScreen();
            if(cws->windowClass == CopyFromParentClass) 
            {
                cws->windowClass = parent->GetClass();
            }
        } else {
            // A root window is being created.
            cws->windowClass = InputOutputClass;
        }

        if(cws->parentId != 0 && cws->windowClass == InputOutputClass)
        {   
            if(parent->GetClass() == InputOnlyClass) {
                cerr << "Initalize: The parent of a InputOutput class can't be a InputOnly window" << endl;
                error->ErrorCode = ErrorStruct::MatchError;
                return -1;
            }
            if(depth == 0) m_depth = parent->GetDepth();
            if(screen && !screen->isVisualValid(cws->visual, depth)) 
            {
                cerr << "XWindow::Initalize: The combination of visual id and depth is not supported on the screen" << endl;
                error->ErrorCode = ErrorStruct::MatchError;
                return -1;
            }
        } else if(cws->windowClass == InputOnlyClass) 
        {
            // check the attributes
            assert(false);
            // the depth must be zero for a input only window.
            if(depth != 0) {
                error->ErrorCode = ErrorStruct::MatchError;
                return -1;
            }
        }
        // Set the attributes.
        if(cws->parentId != 0)
        {
            if(!SetAttributes(cws->mask, cws->values, error, client)) {
                cerr << "SetAttributes() failed" << endl;
                return -1;
            }
            // add this window to the parents child list.
            parent->m_children.push_front(WindowPtr(this));
        }
        // return success
        return 0;
    }

    /* Function:        XWindow::DestroyWindow
     * Description:     Destroys the window and all the childwindows.
     */
    void XWindow::DestroyWindow()
    {
        // Unmap the window if it's mapped
        if(m_mapped) UnmapWindow();
        // destroy the children.
        for(std::list<WindowPtr>::iterator it = m_children.begin(); it != m_children.end(); it++)
            (*it)->DestroyWindow();
        // generate the DestroyNotify event.
        XEvent ev;
        ev.m_type               = XEvent::DestroyNotify;
        ev.destroyNotify.wnd    = m_id;
        // notify the children on this window
        ReportIfClientInterested(ev);
        m_parent->ReportIfClientInterested(ev);
    }

    /* Function:        XWindow::DestroySubwindows
     * Description:     Destroys all the subwindows.
     */
    void XWindow::DestroySubWindows()
    {
        // Destroy all the child windows.
        for(std::list<WindowPtr>::iterator it = m_children.begin(); it != m_children.end(); it++)
            (*it)->DestroyWindow();
        // clear the list
        m_children.clear();
    }

    /* Function:        XWindow::ChangeAttribute
     * Description:     Sets the attributes for the window. Called only by CreateWindow request,
     *                  the non set attributes will be defaulted.
     */
    bool XWindow::SetAttributes(BITMASK bitmask,    // the bitmask
        LISTofVALUE values,                         // the values
        ErrorStruct * error,                        // the error struct
        XClient * client)                           // the client setting the attributes.
    {
        PixmapPtr backgroundPixmap, borderPixmap;
        // need to keep track of this stuff.
        bool bgAllocated = false, borderAllocated = false;

        uint32 index = 0;
        /*if(CountBits(bitmask) != values.size()) {
            // not enough values supplied.
        }*/

        if(bitmask & X_ATTRIBUTE_BACKGROUND_PIXMAP) {
            // a background pixmap was specified.
            // get the value stored in the list
            PIXMAP pixmap = values[index++];
            if(pixmap == 0 /* None */) {
                // no background pixmap
                m_bgPixmap = 0;
            } else if(pixmap == 1 /* ParentRelative */) {
                // get the pixmap from the parent
                if(m_parent->m_depth != m_depth) {
                    // the parent and the window must have the same depth.
                    error->ErrorCode = ErrorStruct::MatchError;
                    goto SetAttributesCleanup;
                }
                // TODO: modify this
                m_bgPixmap = m_parent->m_bgPixmap;
            } else {
                // is the specified pixmap valid?
                XUtility::SmartPointer<XPixmap> ptrPixmap;
                if(!m_server->FindPixmap(pixmap, ptrPixmap, error)) {
                    // could not retrive the pixmap
                    goto SetAttributesCleanup;
                }
                // the pixmap exists, but is it valid? 
                // TODO: check the roots.
                if(ptrPixmap->GetDepth() != this->m_depth) {
                    // a match error
                    error->ErrorCode = ErrorStruct::MatchError;
                    goto SetAttributesCleanup;
                }
            }
        } else {
            // not background pixmap specified. no background pixmap used.
        }

        if(bitmask & X_ATTRIBUTE_BACKGROUND_PIXEL) {
            PixelFormat format;
            // a background pixel was supplied, so create a PIXMAP and fill it with the pixel
            PIXEL pixel = values[index++];
            /*if(!(backgroundPixmap = server->CreatePixmap(m_screen->GetRoot(),4,4,&format))) {
                // could not create the pixmap for the background
                error->ErrorCode = ErrorStruct::AllocError;
                goto SetAttributesCleanup;
            }*/
            // fill the pixmap with the background pixel.
        //  backgroundPixmap->Fill(pixel);
            bgAllocated = true;
        }

        // border pixmap
        if(bitmask & X_ATTRIBUTE_BORDER_PIXMAP) {   
            if(!m_server->FindPixmap(values[index++], borderPixmap, error))
                goto SetAttributesCleanup;
        } else {
            // copy from parent.

        }
        
        // border pixel
        if(bitmask & X_ATTRIBUTE_BORDER_PIXEL)
        {
            PixelFormat format;
            PIXEL borderPixel = values[index++];
        /*  if(!(borderPixmap = server->CreatePixmap(m_screen->GetRoot(), 4,4, &format))) {
                error->ErrorCode = ErrorStruct::AllocError;
                goto SetAttributesCleanup;
            }*/
        }

        // bit gravity
        if(bitmask & X_ATTRIBUTE_BIT_GRAVITY) {
            m_BitGravity = values[index++];
        }

        // window gravoty
        if(bitmask & X_ATTRIBUTE_WIN_GRAVITY) {
            m_WinGravity = values[index++];
        }

        if(bitmask & X_ATTRIBUTE_BACKING_STORE) {
            DWORD backingStore = values[index++];       // ignored for now
        }
    
        if(bitmask & X_ATTRIBUTE_BACKING_PLANES) {
            DWORD backingPlanes = values[index++];      // ignored for now
        }

        if(bitmask & X_ATTRIBUTE_BACKING_PIXEL) {
            DWORD backingPixel = values[index++];       // ignored for now
        }

        if(bitmask & X_ATTRIBUTE_OVERRIDE_REDIRECT) {
            m_overrideRedirect = true;
        }

        if(bitmask & X_ATTRIBUTE_SAVE_UNDER) {
            BOOL saveUnder = values[index++];           // ignored for now.
        }

        if(bitmask & X_ATTRIBUTE_DO_NOT_PROPAGATE_MASK) {
            m_doNotPropagateMask = values[index++];
        }

        /*
         * Check the event mask
         */
        if(bitmask & X_ATTRIBUTE_EVENT_MASK)
        {
            BITMASK events = values[index++];
            if(events & X_EVENT_SUBSTRUCTURE_REDIRECT) {
                if(m_substructureRedirect != NULL) {
                    // generates a access error
                    error->ErrorCode = ErrorStruct::AccessError;
                    goto SetAttributesCleanup;
                } else {
                    m_substructureRedirect = client;
                }
            }
            if(events & X_EVENT_RESIZE_REDIRECT) {
                if(m_resizeRedirect != NULL) {
                    // generates a access error
                    error->ErrorCode = ErrorStruct::AccessError;
                    goto SetAttributesCleanup;
                } else {
                    m_resizeRedirect = client;
                }
            }

            if(events & X_EVENT_BUTTON_PRESS) {
                if(m_buttonPress != NULL) {
                    error->ErrorCode = ErrorStruct::AccessError;
                    goto SetAttributesCleanup;
                }
                m_buttonPress = client;
            }
            m_clientInterests.push_back(std::pair<BITMASK, XClient *>(events, client));
        }

        m_bgPixmap      = backgroundPixmap;
        m_borderPixmap  = borderPixmap;
        return true;

    SetAttributesCleanup:
        // cleanup code.
        if(bgAllocated) {
            // free the pixmap
            m_server->FreePixmap(m_bgPixmap->GetId(), error);
            m_bgPixmap = 0;
        }
        if(borderAllocated) {
            // free the pixmap
            m_server->FreePixmap(m_borderPixmap->GetId(), error);
            m_borderPixmap = 0;
        }
        return false;
    }

    /* Function:        XWindow::GetAttributes
     * Description:     Retrives the attributes for the window.
     */
    void XWindow::GetAttributes(AttributeStruct * attributes, const XClient * client) const
    {
        // Get all the events 
        attributes->AllEventMasks   = 0;
        attributes->yourEventMasks  = 0;
        for(list<pair<BITMASK, XClient *> >::const_iterator it = m_clientInterests.begin();
            it != m_clientInterests.end();
            it++)
        {
            attributes->AllEventMasks |= it->first;
            if(it->second == client) attributes->yourEventMasks = it->first;
        }
    }
};