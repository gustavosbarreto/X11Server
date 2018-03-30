#include "XWindow.h"
#include <queue>
#include "XServer.h"

using namespace std;
using namespace XUtility;

namespace X
{
    /* Function:        XWindow::isAncestor
     * 
     */
    bool XWindow::isAncestor(const WindowPtr & wnd)
    {
        WindowPtr current = m_parent;
        while(current.valid()) {
            if(current == wnd) return true;
            current = current->m_parent;
        }
        return false;
    }

    /* Function:        XWindow::isChild
     *
     */
    bool XWindow::isChild(const WindowPtr & wnd)
    {
        WindowPtr rhs(this);
        return wnd->isAncestor(rhs);
    }
    
    /* Function:        XWindow::GetLeastCommonAncestor
     *
     */
    WindowPtr XWindow::GetLeastCommonAncestor(const WindowPtr & wnd1, const WindowPtr & wnd2) const
    {
        if(wnd1->GetScreen() != wnd2->GetScreen())
            // Not on the same screen
            return WindowPtr(0);

        if(wnd1->isChild(wnd2) || wnd1->isAncestor(wnd2))
            // the windows are directly related.
            return WindowPtr(0);

        WindowPtr current = wnd1->m_parent;
        while(current.valid()) {
            if(current->isAncestor(wnd2)) return current;
            current = current->m_parent;
        }
        // No least common ancestor. Should never happen.
        return WindowPtr(0);
    }

    /* Function:        XWindow::GetAncestors
     * Description:     Retrives all the ancestors for this window.
     */
    void XWindow::GetAncestors(std::list<WindowPtr> & ancestors)
    {
        SmartPointer<XWindow> current = m_parent;
        while(current.valid()) {
            ancestors.push_back(current);
            current = current->m_parent;
        }
    }

    
    /* Function:        XWindow::GetNearestAncestorNotCreatedByClient
     * Description:     The name says it all.
     */
    WindowPtr XWindow::GetNearestAncestorNotCreatedByClient(XClient * client)
    {
        WindowPtr current = this->m_parent;
        while(current.valid())
        {
            if(current->m_creator != client) {
                return current;
            } 
            current = current->m_parent;
        }
        // should never happen
        return WindowPtr(0);
    }

    /* Function:        XWindow::GetVisibilityStatus
     * Description:     Calculates the current visibility status of the window.
     */
    /*typedef list<XRectangle> RectLst;
    typedef RectLst::iterator RectLstIt;
    int XWindow::GetVisibilityStatus(RectLst & rects)
    {
        bool fullyVisible = true;
        if(isRoot()) {
            // the entire root window is visible
            rects.push_back(m_rect);
            return 1;
        }
        RectLst parentAreas;
        int nret = m_parent->GetVisibility(parentAreas);
        if(nret == -1) {
            // the parent is not visible
            return -1;
        }
        // The outer rectangle of the 
        XRectangle prect = m_parent->GetActualClippedRect();
        // Calculate the actual position and size of the outer "frame".
        XRectangle rect = m_borderRect.Translate(prect.m_x, prect.m_y);
        // Does this overlap with the parents visible area?
        nret = prect.GetOverlappedAreas(rect, &visibleAreas);
        if(nret == -1) {
            // no parts of the window is visible on the screen
        } else if(nret == 0) {
            // the entire window is not visible
            fullyVisible = false;
        } else {
            // was the window clipped? 
        }
        // now check all the siblings.
        RectLst siblingAreas;   
        // Find the position of this window among the siblings.
        list<WindowPtr>::iterator it = 
            find(m_parent->m_children.begin(), m_parent->m_children.end(), WindowPtr(this));
        
        if(it == m_parent->m_children.begin()) {
            // the window is first among the siblings, so just add all the visible parts of it.
            copy(visibleAreas.begin(), visibleAreas.end(), back_insert_iterator(rects));
            return (fullyVisible ?  1 : 0);
        } else {
            // loop thru all the siblngs above me in the stack order.
            for(list<WindowPtr>::iterator sibling = m_parent->m_children.begin(); 
                sibling != it;
                ++sibling)
            {
                // get the rectangle for the sibling
                XRectangle rect = (*sibling)->GetRect();
                // we need to translate it relative the parents rectangle
                rect = rect->TranslateRelative(prect);
                // does this overlap with this window?
                if(!rect.overlaps(thisRect)) continue;
                // yes, it overlaps so add the rectangle to the list
                siblingAreas.push_back(rect);
            }
            if(siblingAreas.empty()) return 1;  // the window is completely visible
            else {
                RectLst nonObscured;
                // Need to check all the "visible areas" of the window against the overlapping sibling areas
                for(RectLstIt siblingRect = siblingAreas.begin();siblingRect != siblingAreas.end(); siblingRect++)
                    for(RectLstIt vIt = visibleAreas.begin(); vIt != visibleAreas.end(); vIt++) 
                        siblingRect->GetNonObscuredAreas(*vIt, &nonObscured);
                // copy the non-overlapping areas, this is the visible areas of the window.
                copy(nonObscured.begin(), nonObscured.end(), back_insert_iterator(rects));
                return (nonObscured.empty() -1 : 0);
            }       
        }
    }*/

    /* Function:        XWindow::GetWindowAt
     * Description:     Retrives the window at the position. The position is relative the
     *                  Parents local origin.
     */
    bool XWindow::GetWindowAt(int x, int y, WindowPtr & ptr)
    {
        if(!isVisible()) return false;
        if(m_insideRect.IsInside(x,y))
        {
            // for all the children in top to bottom stacking order.
            for(WindowLstIt it = m_children.begin(); it != m_children.end(); it++)
            {
                if((*it)->isVisible() && (*it)->IsInside(x, y)) {
                    // need to modify the position so it's relative this window.
                    return (*it)->GetWindowAt(x-m_insideRect.m_x, y-m_insideRect.m_x, ptr);     
                }
            }
        }
        if(m_borderRect.IsInside(x,y))
        {
            // inside the outer border
            ptr = this;
            return true;
        }
        return false;
    }

    /* Function:        XWindow::IsInside
     * Description:     Returns true if the point is inside the windows outer rectangle, that is including the 
     *                  border area.
     */
    bool XWindow::IsInside(int x, int y)
    {
        return m_borderRect.IsInside(x,y);
    }
};