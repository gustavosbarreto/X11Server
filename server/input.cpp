#include "XServer.h"
#include "PointerData.h"
#include <iostream>

namespace X
{
    using namespace std;
    using namespace XUtility;

    /* Function:        XServer::MouseMoved
     * Description:     Called when the mouse is moved.
     */
    void XServer::MouseMoved(int x,     // the x position of the pointer 
        int y,                          // the y position of the pointer 
        unsigned int mask)
    {
        PointerData pd;
        pd.m_type           = PointerData::PointerMoveEvent;
        pd.Movement.ts      = ServerTime();     // Get the current server time
        pd.Movement.x       = x;
        pd.Movement.y       = y;
        pd.Movement.state   = m_keyboard->GetModifierState();   // Get the current state of the modifiers.
        pd.m_screen         = m_activeScreen;

        cerr << "Mouse moved to position: " << x << "," << y << endl;
        // add the event to the buffer,
        m_motionBuffer.push_back(pd);
        if(m_motionBuffer.size() > 256) m_motionBuffer.pop_front();
        // now let the proxy delegate the event.
        m_grabProxy->PointerEvent(pd);
    }

    /* Function:        XServer::ButtonPressed
     * Description:     Called when a mouse button is pressed.
     */
    void XServer::ButtonPressed(int x, 
        int y, 
        BUTTON button, 
        unsigned int mask)
    {
        PointerData pd;
        pd.m_type       = PointerData::ButtonPressEvent;
        pd.Press.button = button;
        pd.Press.state  = mask;
        pd.Press.ts     = ServerTime();
        pd.Press.x      = x;
        pd.Press.y      = y;
        pd.m_screen     = m_activeScreen;
        // add the event to the buffer,
        m_motionBuffer.push_back(pd);
        if(m_motionBuffer.size() > 256) m_motionBuffer.pop_front();
        // now let the grab proxy handle this
        this->m_grabProxy->PointerEvent(pd);
    }

    /* Function:        XServer::GenerateLeaveAndEnterEvents
     * Description:     Generates the required leave and enter windows.
     */
    /*void XServer::GenerateLeaveAndEnterEvents(XUtility::SmartPointer<XWindow> & A,        // the old window
                                                XUtility::SmartPointer<XWindow> & B,    // the current window.
                                                int x,                                      // the x position
                                                int y)                                      // the y position.
    {
        SmartPointer<XWindow> current;
        XEvent ev;

        if(A->isChild(B)) {

            leave.root_x        = x;
            leave.root_y        = y;
            leave.rootWnd       = rootWnd;
            leave.samescreen    = 1;

            // When the pointer moves from window A to window B and A is an inferior of B
            CrossingEvent & leave = ev.crossingEvent;
            leave.detail        = LeaveNotifyEvent::Ancestor;
            m_grabProxy->OnPointerEvent(ev, A);

            // Generate Leave notify on all windows between A and B
            current = A->GetParent();
            while(current != B) {
                leave.detail = LeaveNotifyEvent::Virtual;
                m_grabProxt->OnPointerEvent(ev, current);
            }
            // Generate a EnterNotify event on B with detail 'inferior'.
            EnterNotify & enter = ev.enterNotify;
            enter.detail        = EnterNotifyEvent::Inferior;
        } else if(B->isChild(A)) {
            // When the pointer moves from window A to window B and B is an inferior of A
            LeaveNotifyEvent & leave    = ev.crossingEvent;
            leave.detail                = LeaveNotifyEvent::Inferior;
            m_grabProxy->OnPointerEvent(ev, A);

            // For all the windows between A and B.


        } else {
            // When the pointer moves from window A to window B and window C is their least common ancestor
            // get the least common ancestor
            SmartPointer<XWindow> lca = A->GetLeastCommonAncestor(A, B);
            // for all the windows between A and C, generate a LeaveNotify event, in this order.
            current = A->GetParent();
            while(current != lca) {
                // Leave Notify with detail Nonlinear Virtual
                LeaveNotify & leave = ev.crossingEvent;
                leave.detail        = LeaveNotifyEvent::Nonlinear;
                m_grabProxy->OnPointerEvent(ev, current);
            }

            // for all the window between C and B, generate a EnterNotify event, in this order.
    
            // Generate a EnterNotify event on B with detail Nonlinear.
            
        }

    }*/
};