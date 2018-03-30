#include "XWindow.h"
#include "XDefines.h"
#include "SmartPointer.h"
#include "XServer.h"
#include "common.h"
#include <assert.h>
#include <list>
#include <iostream>
#include "GLRenderer.h"

using namespace std;
using namespace XUtility;

namespace X
{
    
    /* Function:        XWindow::ReportIfClientInterested
     * Description:     Reports a event to a client if the client is interested in it.
     */
    void XWindow::ReportIfClientInterested(XEvent & ev)
    {
        BITMASK bit = 0;
        switch(ev.m_type)
        {
        case XEvent::MapNotify:             // A window was mapped
        case XEvent::UnmapNotify:           // A window was unmapped.
            bit = (ev.mapNotify.wnd == m_id) ? X_EVENT_STRUCTURE_NOTIFY : X_EVENT_SUBSTRUCTURE_NOTIFY;
            ev.mapNotify.eventWnd = m_id;   // set the event window
            break;
        case XEvent::DestroyNotify:             // a window was destroyed
            bit = (ev.destroyNotify.wnd == m_id) ? X_EVENT_STRUCTURE_NOTIFY : X_EVENT_SUBSTRUCTURE_NOTIFY;
            ev.destroyNotify.eventWnd = m_id;   // set the event window.
            break;

        default:
            break;
        }       
        // for all the clients.
        for(list<pair<BITMASK, XClient *> >::iterator it = m_clientInterests.begin(); 
            it != m_clientInterests.end(); 
            it++)
        {
            if((it->first) & bit) it->second->AddEvent(ev);
        }
    }
    
    /*
     *
     */
    void XWindow::SetBackground(PixmapPtr & bg)
    {
        m_bgPixmap = bg;
    }
    /*
     *
     */
    BITMASK XWindow::GetClientInterests(const XClient * client) const
    {
        for(list<pair<BITMASK, XClient *> >::const_iterator it = m_clientInterests.begin();
            it != m_clientInterests.end();
            it++)
        {
            if(client == it->second)
            {
                return it->first;
            }
        }
        return 0;
    }

    /* Function:        XWindow::isVisible
     * Description:     Returns true if the current window is visible.
     */
    bool XWindow::isVisible() const
    {
        if(!m_mapped) return false;
        WindowPtr current = m_parent;
        while(current.valid()) {
            if(!current->m_mapped) return false;
            current = current->m_parent;
        }
        return true;
    }

    /* Function:        XWindow::RemoveChild
     * Description:     Removes a child from the window's list with children.
     */
    void XWindow::RemoveChild(WindowPtr child)
    {
        m_children.remove(child);
    }

    /* Function:        XWindow::NowVisible
     * Description:     Called when the window is visible.
     */

    void XWindow::NowVisible()
    {
        /*XEvent ev;
        ev.m_type = XEvent::VisibilityNotify;
        ev.visibilityNotify.wnd = m_id;
        // The window is visible
        int nret = GetVisibilityStatus();
        switch(nret)
        {
        case 1:
            // The window is Unobscured
            ev.visibilityNotify.state = VisibilityNotifyEvent::Unobscured;
            break;
        case 0:
            // the window is PartiallyObscured
            ev.visibilityNotify.state = VisibilityNotifyEvent::PartiallyObscured;
            break;
        case -1:
            // the window is FullyObscured.
            ev.visibilityNotify.state = VisibilityNotifyEvent::FullyObscured;
        }
        // Now report it to the interested clients
        ReportIfClientInterested(ev);
        // Need to tell all the children also
        for(WindowLstIt it = m_children.begin(); it != m_children.end(); it++)
            if((*it)->m_mapped) (*it)->NowVisible();
        WindowLst & ptrList = m_parent->m_children;
        // Need to check the visibility status of all the siblings behind this one.
        */
    }


    /* Function:        XWindow::Resize
     * Description:     Resizes the window.
     */
    int XWindow::Resize(uint32 width, uint32 height)
    {
        int32 dx = width - m_rect.m_width, dy = height - m_rect.m_height;
        // The window is resized, reposition the children.
        for(list<WindowPtr>::iterator it = m_children.begin(); it != m_children.end();it++)
            // reposition the window relative the parent.
            (*it)->Reposition(dx, dy);
        return 0;
    }

    /* Function:        XWindow::Reposition
     * Description:     Repositions the window relative the parent using the window gravity.
     */
    void XWindow::Reposition(int16 dw, int16 dh)
    {   
        int dx = 0, dy = 0;
        switch(m_WinGravity)
        {
        case WinNorthWest:
            break;
        case WinNorth:
            dx = dw / 2;
            break;
        case WinNorthEast:
            dx = dw;
            break;
        case WinWest:
            dy = dh / 2;
            break;
        case WinCenter:
            dy = dh / 2;
            dx = dw / 2;
            break;
        case WinEast:
            dy = dw;
            dx = dh / 2;
            break;  
        case WinSouthWest:
            dy = dh;
            break;
        case WinSouth:
            dx = dw / 2;
            dy = dh;
            break;
        case WinSouthEast:
            dx = dw;
            dy = dh;
            break;
        }
    }

    /* Function:        XWindow::DrawWindow
     * Description:     Draws the actual window.
     */
    void XWindow::DrawWindow(int x,int y, int w, int h)
    {   
        if(m_borderWidth > 0)
        {
            // The window has a border, so tile it.
            if(!m_borderPixmap.valid())
            {
                cerr << "XWindow::DrawWindow(): the window should have a border, but the pointer is invalid" << endl;
                assert(false);
            }
            XRectangle outer;
            outer.m_x = m_rect.m_x + x - m_borderWidth;         // the outer x
            outer.m_y = m_rect.m_y + y - m_borderWidth;         // the outer y
            outer.m_width = m_rect.m_width + (m_borderWidth << 1);      // the outer width,
            outer.m_height = m_rect.m_height + (m_borderWidth << 1);    // the outer height

            GLRenderer::TileArea(m_borderPixmap->GetSurface(), outer.m_x, outer.m_y, outer.m_width, outer.m_height);
        }
        // now render the background
        if(!m_bgPixmap) {
            cerr << "XWindow::DrawWindow(): The window does not have a background pixmap" << endl;
            assert(false);
        }

        
        // now draw the background.
        XRectangle inner;
        inner.m_x = x + m_rect.m_x;
        inner.m_y = y + m_rect.m_y;
        inner.m_width = m_rect.m_width;
        inner.m_height = m_rect.m_height;

        GLRenderer::TileArea(m_bgPixmap->GetSurface(), inner.m_x, inner.m_y, inner.m_width, inner.m_height);
        //GLRenderer::Draw(m_bgPixmap.ptr(), inner);
    }

    /* Function:        XWindow::FindMatchingButtonGrab
     *
     */
    bool XWindow::FindMatchingButtonGrab(BUTTON button,
                                        SETofKEYMASK mask,
                                        ButtonGrabStruct * grab,                    
                                        XClient ** grab_client)
    {
        // need to get the ancestors of this window
        list<WindowPtr> wndlist;
        GetAncestors(wndlist);
        for(list<WindowPtr>::reverse_iterator rit = wndlist.rbegin(); rit != wndlist.rend(); ++rit)
        {
            // does this window have a matching passive grab?
            const list<PassiveGrab> & grabs = (*rit)->m_grabs;
            for(list<PassiveGrab>::const_iterator it = grabs.begin(); it != grabs.end(); it++)
            {
                if(it->m_type != PassiveGrab::ButtonGrab) 
                    continue;   // not a passive button grab.
                if(it->buttongrab.button == 0) /* Any button */ {
                    // any button will match the grab requirements
                    if(it->buttongrab.modifiers == 0x8000) /* Any modifiers. */{
                        // any button and modifier will due, so activate this passive grab.
                        *grab           = it->buttongrab;
                        *grab_client    = it->m_client;
                        return true;
                    } else {
                        if((it->buttongrab.modifiers & grab->modifiers) == it->buttongrab.modifiers) {
                            // the modifiers matches, so activate this passive grab
                            *grab           = it->buttongrab;
                            *grab_client    = it->m_client; 
                            return true;
                        }
                    }
                } else if(it->buttongrab.button == grab->button)
                {
                    // The button matches, but does the modifier matches?
                    if(it->buttongrab.modifiers == 0x8000) /* Any modifiers */ {
                        // any modifier will do
                        *grab           = it->buttongrab;
                        *grab_client    = it->m_client; 
                        return true;
                    } else 
                    {
                        if((it->buttongrab.modifiers & grab->modifiers) == it->buttongrab.modifiers) {
                            // the modifiers matches, so activate this passive grab
                            *grab           = it->buttongrab;
                            *grab_client    = it->m_client; 
                            return true;
                        }
                    }
                }
            }
        }
#if defined(_DEBUG_VERBOSE)
        cerr << "XWindow::FindMatchingButtonGrab (debug): No matching passive grab found for the button press" << endl;
#endif
        return false;
    }
};
