#include "XWindow.h"
#include <list>
#include "IInputProxy.h"
#include "XDefines.h"

namespace X
{

    using namespace std;
    /* Function:        XWindow::PropagatePointerEvent
     * Description:     Handles a pointer event.
     */
    void XWindow::PropagatePointerEvent(XEvent & ev, WINDOW srcId, WINDOW callId)
    {
        /*std::list<XClient *> clients;
        switch(ev.m_type)
        {
        case XEvent::LeaveNotify:   // The pointer left a window.
        case XEvent::EnterNotify:   // the pointer entered a window.
            {
                clients = GetInterestedClients(XEvent::EnterNotify);
                if(clients.empty()) {
                    // no interested clients.
                    if(PropagateEvent(ev.m_type)) {
                        // propagate the event to the parent.
                        m_parent->PropagatePointerEvent(ev, srcId, GetId());
                    }
                } else {
                    // found interested clients, this is the event window.

                }
                break;
            }
        case XEvent::MotionNotify:  // Rewrite this later.
            {
                // Update the event with the informatio about the event window.
                MotionNotifyEvent & notify = ev.motionNotify;
                notify.eventWnd = GetId();
                // Set the correct child window.
                notify.childWnd = (srcId == notify.eventWnd ? 0 : callId);
                
                bool foundClient = false;
                // for all the clients.
                for(list<pair<BITMASK, XClient *> >::iterator it = m_clientInterests.begin();
                    it != m_clientInterests.end();
                    it++)
                {
                    BITMASK bitmask = it->first;
                    if(bitmask & POINTER_MOTION_EVENT) {
                        // Add the event to the clients event queue.
                        XClient * client = it->second;
                        client->AddEvent(ev);
                        foundClient = true;
                    } 
                    else if(bitmask & BUTTON_1_MOTION_EVENT || 
                        bitmask & BUTTON_2_MOTION_EVENT ||
                        bitmask & BUTTON_1_MOTION_EVENT)
                    {
                        // The client is only interested in the motion notify
                        // event if one or more of the mouse buttons are pressed.
                        if(((notify.keybutmask & X_LEFT_BUTTON_PRESSED) && (bitmask & BUTTON_1_MOTION_EVENT)) ||
                            ((notify.keybutmask & X_RIGHT_BUTTON_PRESSED) && (bitmask & BUTTON_1_MOTION_EVENT)) ||
                            ((notify.keybutmask & X_MIDDLE_BUTTON_PRESSED) && (bitmask & BUTTON_1_MOTION_EVENT)))
                        {
                            // Add the event to the clients event queue.
                            XClient * client = it->second;
                            client->AddEvent(ev);
                            foundClient = true;
                        }
                    }
                }
                // should the event be propagated upwards?
                if(foundClient == false && PropagateEvent(ev.m_type)) {
                    m_parent->PropagatePointerEvent(ev, srcId, GetId());
                }
            }
        }*/

    }
}
