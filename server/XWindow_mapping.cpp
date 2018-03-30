/* File:            XWindow_mapping.cpp
 * Description:     Contains all the functions regarding mapping and
 *                  unmapping of windows.
 * Author:          Magnus Leksell
 *
 * Copyright 2007-2008 Magnus Leksell, all rights reserved
 */

#include "XWindow.h"

using namespace std;

namespace X
{
    /* Function:        XWindow::MapWindow
     * Description:     Maps the current window. Notifies the children if the window is currently visible.
     *                  The current implementation is always maintaining a backing store, so no expose events
     *                  is generated.
     */
    int XWindow::MapWindow(XClient * client)
    {
        if(isRoot())
        {
            if(client != NULL) return -1; // a client can't map the root window
            else {
                // but the server can. Should we reallocate the offscreen buffer?
                if(m_mapped) return 0;  // the window is already mapped.
                // the window is not mapped
                if(m_mapType == WhenMapped) {
                    // We need to allocate the offscreen rendering surface.
                    m_mapped = true;
                    return 0;
                }
            }
        }
        if(m_mapped == false) 
        {
            // The window is not mapped
            if(m_overrideRedirect == false  && // over-redirect is false
                (m_parent->m_substructureRedirect && (m_parent->m_substructureRedirect != client)))
            {
                // The window is not mapped, but a a map request event is generated.
                XEvent ev;
                ev.m_type               = XEvent::MapRequest;
                ev.mapRequest.wnd       = m_id;
                ev.mapRequest.parent    = m_parent->m_id;
                m_parent->m_substructureRedirect->AddEvent(ev);
            } else {
                // the window is mapped
                m_mapped = true;
                // the window is mapped and a map notify event is generated.
                XEvent ev;
                ev.m_type                       = XEvent::MapNotify;
                ev.mapNotify.wnd                = m_id; 
                ev.mapNotify.overrideRedirect   = m_overrideRedirect;
                // Report it to the interested clients.
                ReportIfClientInterested(ev);
                m_parent->ReportIfClientInterested(ev);
            
                // Is the window visible?
                if(m_parent->isVisible()) 
                    // Generate any required VisibilityNotify events.
                    NowVisible();
            }
        }
        return 0;
    }

    /* Function:        XWindow::UnmapWindow
     * Description:     Unmaps the current window. Will notify all the children that the
     *                  client was unmapped.
     */
    int XWindow::UnmapWindow(bool FromConfigure)
    {
        if(m_mapped == true)
        {
            // the window is mapped.
            XEvent ev;
            ev.m_type                       = XEvent::UnmapNotify;
            ev.unmapNotify.wnd              = m_id;
            ev.unmapNotify.fromConfigure    = FromConfigure;
            
            // Report it to the interested clients.
            ReportIfClientInterested(ev);
            m_parent->ReportIfClientInterested(ev);
            
            // Now calculate the visibility of the affected windows.
            //NowNotVisible();
        }
        return 0;
    }

    /* Function:        UnmapSubWindows
     * Description:     Unmaps all the child windows.
     */
    int XWindow::UnmapSubWindows()
    {
        // Unmap all the child windows in top to bottom stacking order.
        for(list<WindowPtr>::iterator it = m_children.begin(); it != m_children.end(); it++)
            if((*it).valid()) (*it)->UnmapWindow();
        return 0;
    }

    /* Function:        XWIndow::MapSubwindows
     * Description:     Performs a MapWindow request on all the children, int bottom to top 
     *                  stacking order.
     */
    int XWindow::MapSubWindows(XClient * client)
    {
        for(list<WindowPtr>::reverse_iterator rit = m_children.rbegin();rit != m_children.rend();rit++)
            (*rit)->MapWindow(client);
        return 0;
    }

};