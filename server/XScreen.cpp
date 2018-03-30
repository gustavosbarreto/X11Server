#include "XScreen.h"
#include "XRectangle.h"
#include "XServer.h"

namespace X
{
    /* Function:        XScreen::XScreen
     * Description:     Constructor, initalizes the screen.
     */
    XScreen::XScreen(XServer * server,      // the x server
        WINDOW wId,                         // the root window
        const DepthList & depths)           // the supported depths.
        : m_depths(depths)
    {
    }

    /* Function:        XScreen::isVisualValid
     * Description:     Returns true if the combination of the visual type and depth is
     *                  supported by this screen.
     */
    bool XScreen::isVisualValid(VISUALID id, uint32 depth) const
    {
        for(std::list<DepthType>::const_iterator it = m_depths.begin();
            it != m_depths.end();
            it++)
        {
            if(it->depth == depth)
            {
                // found the depth, is the visual id supported
                for(std::list<VisualType>::const_iterator vIt = it->visuals.begin();
                    vIt != it->visuals.end();
                    vIt++)
                {
                    if(id == vIt->id)
                        return true;
                }
                // the visual id is not supported for this depth
                return false;
            }
        }
        // the depth is not supported
        return false;
    }

    /* Function:        XScreen::GetRoot
     * Description:     Returns the identifier for the root window.
     */
    WINDOW XScreen::GetRoot() 
    {
        return m_rootId;
    }

    /* Function:    XScreen::GetWindowAt
     *
     */
    bool XScreen::GetWindowAt(int x, int y, WindowPtr & wnd)
    {
        m_root->GetWindowAt(x, y, wnd);
        return true;
    }

    /*
     *
     */
    void XScreen::SetRootWindow(WindowPtr wnd)
    {
        m_root = wnd;
        m_rootId = wnd->GetId();
    }
};