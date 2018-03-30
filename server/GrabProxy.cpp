#include "GrabProxy.h"
#include "XWindow.h"
#include "common.h"
#include <assert.h>
#include "XScreen.h"
#include "XDefines.h"
#include <iostream>
#include "XServer.h"

namespace X
{

    using namespace XUtility;
    using namespace std;
    
    /* Function:        GrabProxy::GrabProxy
     * Description:     Constructor, performs the required initalization.
     */
    GrabProxy::GrabProxy(XServer * server, XKeyboard * keyboard, XPointer * pointer)
        : m_server(server), m_keyboard(keyboard), m_pointer(pointer)
    {
    }

    /* Function:        GrabProxy::AllowEvents
     * Description:     Frees some queued events.
     */
    int GrabProxy::AllowEvents(int mode, TIMESTAMP ts, XClient * client, ErrorStruct * error)
    {
        switch(mode)
        {
        case ASYNC_POINTER:
            if(m_info.isPointerGrabbed())   // the pointer is grabbed.
            {
                // is the pointer freezed by the client
                if(m_info.isPointerFrozen() && m_info.PointerGrab.freezeClient == client)
                {
                    // The pointer events was freezed by the client.
                    // The events are processed as normal
                    m_info.PointerGrab.freezeClient = NULL;
                    m_info.PointerGrab.mode = GrabInfo::Async;
                    // Now handle the queued events.
                    ProcessQueuedPointerEvents();
                }
            }
            break;
        case SYNC_POINTER:
            if(m_info.isPointerGrabbed() && m_info.isPointerFrozen() && m_info.PointerGrab.freezeClient == client)
            {
                // Continue until a button release or button press is processed.
                m_info.PointerGrab.mode = GrabInfo::Async_UntilButtonEvent;
                ProcessQueuedPointerEvents();
            }
            break;
        case REPLAY_POINTER:
            cerr << "GrabProxy::AllowEvents: Replay pointer is not implemented yet" << endl;
            assert(false);
            break;
        case ASYNC_KEYBOARD:
            if(m_info.isKeyboardGrabbed() &&                    // the keyboard is grabbed
                m_info.isKeyboardFrozen() &&                    // the keyboard is frozen 
                m_info.KeyboardGrab.freezeClient == client)     // by the client
            {
                // processing of keyboard events continue normally.
                m_info.KeyboardGrab.freezeClient = NULL;
                m_info.KeyboardGrab.mode = GrabInfo::Async;
                //ProcessQueuedKeyboardEvents();
            }
            break;
        case SYNC_KEYBOARD:
            if(m_info.isKeyboardGrabbed() &&                    // the keyboard is grabbed
                (m_info.KeyboardGrab.grabClient == client) &&   // by the client
                m_info.isKeyboardFrozen() &&                    // and the keyboard is frozen
                (m_info.KeyboardGrab.grabClient == client))     // by the client.
            {
                // Process the events until a keyrelease or keypress event.
                m_info.KeyboardGrab.mode = GrabInfo::Async_UntilKeyEvent;
                //ProcessQueuedKeyboardEvents();
            }
            break;
        case REPLAY_KEYBOARD:
            cerr << "GrabProxy::AllowEvents: Replay keyboard is not implemented yet" << endl;
            assert(false);
            break;
        case ASYNC_BOTH:
            if(m_info.isKeyboardGrabbed() && m_info.isPointerGrabbed() &&
                m_info.isKeyboardFrozen() && m_info.isPointerFrozen() &&
                m_info.KeyboardGrab.freezeClient == client && m_info.PointerGrab.freezeClient == client)
            {
                // both the keyboard and pointer is frozen by the client.
                m_info.KeyboardGrab.mode = GrabInfo::Async_UntilDeviceEvent;
                m_info.PointerGrab.mode = GrabInfo::Async_UntilDeviceEvent;
                // Process the queued keyboard and pointer events in the correct order (using the timestamp).
                //ProcessQueuedEvents();
            }
        case SYNC_BOTH:
            break;
        }
        return 0;
    }

    /* Function:        GrabProxy::ProcessQueuedPointerEvents
     * Description:     Processes some queued pointer events.
     */
    void GrabProxy::ProcessQueuedPointerEvents()
    {
        while(!m_pointerQueue.empty())
        {
            assert(m_info.PointerGrab.mode != GrabInfo::Synch);
            // Get the first from the queue
            PointerData pd = m_pointerQueue.front(); m_pointerQueue.pop();
            // Handle the event
            HandleGrabbedPointerEvent(pd);
            // was the pointer event processing freezed by the event?
            if(m_info.PointerGrab.mode == GrabInfo::Synch)
                // The event processing was freezed, leave the remaining events in the queue.
                return;
        }
    }

    /* Function:        GrabProxy::HandlePointerEvent
     * Description:     Handles pointer events while the pointer is grabbed.
     */
    void GrabProxy::HandleGrabbedPointerEvent(const PointerData & pd)
    {
        XEvent ev;
        switch(m_info.PointerGrab.mode)
        {
        case GrabInfo::Async_UntilButtonEvent:
            // We should continue with the event processing until a button is released/pressed.
            if(pd.m_type == PointerData::PointerMoveEvent)
            {
            } else if(pd.m_type == PointerData::ButtonPressEvent)
            {
                // Send the event, then freeze the processing
                ev.m_type = XEvent::ButtonPressed;
                ev.pointerEvent.button      = pd.Press.button;
                ev.pointerEvent.timestamp   = pd.Press.ts;

                // add the event to the clients queue
                m_info.PointerGrab.grabClient->AddEvent(ev);
                // and freeze the processing
                m_info.PointerGrab.mode = GrabInfo::Synch;
            } else if(pd.m_type == PointerData::ButtonReleaseEvent)
            {
                // A button was released.
                if((m_info.PointerGrab.reason == GrabInfo::Passive ||
                    m_info.PointerGrab.reason == GrabInfo::AutoGrab) &&
                    pd.Press.AllButtonsReleased())
                {
                    // This will terminate the grab.
                    //TerminatePointerGrab();
                } else {
                    // freeze the processing
                    m_info.PointerGrab.mode = GrabInfo::Synch;
                }
            }
        break;
        case GrabInfo::Async:
            // handle the event normally.
            break;
        case GrabInfo::Async_UntilDeviceEvent:
            if(pd.m_type == PointerData::ButtonPressEvent || pd.m_type == PointerData::ButtonReleaseEvent)
            {
                // freeze the keyboard and pointer.
                m_info.PointerGrab.mode = GrabInfo::Synch;
                m_info.KeyboardGrab.mode = GrabInfo::Synch;
            } else {
                // process the event as normal
            }
            break;
        case GrabInfo::Synch:
            // Add the event to the queue.
            m_pointerQueue.push(pd);
            break;
        default:
            cerr << "GrabProxy::HandlePointerEvent: Unknown pointer mode" << endl;
            assert(false);
        }
    }

    /* Function:        GrabProxy::HandlePointerEvent
     * Description:     Handles a pointer event when the pointer isn't grabbed.
     */
    void GrabProxy::HandlePointerEvent(const PointerData & pd)
    {
        assert(m_info.isPointerGrabbed() == false);
        switch(pd.m_type)
        {
        case PointerData::PointerMoveEvent:
            {
                WindowPtr wnd;
                pd.m_screen->GetWindowAt(pd.Movement.x, pd.Movement.y, wnd);
                if(!(wnd == m_oldWnd)) {
                    // The active window changed.
                    if(wnd->GetScreen() != m_oldWnd->GetScreen()) {
                        // The pointer changed screen.
                    } else {
                        // The pointer is still on the same screen.
                    }
                }
            }
        case PointerData::ButtonPressEvent:
            {
                // This will either activate a passive grab, or perform a "autograb"
                // first get the window the pointer is in.
                WindowPtr wnd;
                pd.m_screen->GetWindowAt(pd.Movement.x, pd.Movement.y, wnd);
                ButtonGrabStruct bgs;
                XClient * client;
                if(wnd->FindMatchingButtonGrab(pd.Press.button, pd.Press.state, &bgs, &client)) {
                    // found a matching grab
                    if(!ActivateButtonGrab(&bgs, client)) {
                        cerr << "GrabProxy::HandlePointerEvent: Failed to activate a passive grab" << endl;
                        return;
                    }
                } else {
                    // no matching grab. Find the window that has a client interested in the event
                    WindowPtr eventWnd;
                    XClient * client = 0;
                    assert(false);
                //  if(!wnd->FindClientInterestedInButtonPress(eventWnd, &client))
                        // no interested client, can't activate a grab
                //      return;
                    // get the bitmask of the events the client is interested in.
                    BITMASK mask = wnd->GetClientInterests(client);
                    // found a window with a interested client.
                    bgs.confineTo       = 0;
                    bgs.cursor          = 0;
                    bgs.pointerMode     = GrabInfo::Async;
                    bgs.keyboardMode    = GrabInfo::Async;
                    bgs.ownerEvents     = (mask & X_EVENT_OWNER_GRAB_BUTTON ? 1 : 0);
                    bgs.eventMask       = mask;
                    bgs.modifiers       = 0;
                    // now activate the grab
                    if(!ActivateButtonGrab(&bgs, client)) {
                        cerr << "GrabProxy::HandlePointerEvent: Failed to activate a grab" << endl;
                        return;
                    }
                }
            }
        case PointerData::ButtonReleaseEvent:
            // should only happen if we failed to set the previous grab, so ignore it.
            return;
        default:
            break;
        }
    }

    /* Function:        GrabProxy::PointerMoved
     * Description:     Called when the pointer is moved.
     */
    void GrabProxy::PointerEvent(PointerData & pd)
    {
        if(m_info.isPointerGrabbed()) {
            // The pointer is grabbed
            HandleGrabbedPointerEvent(pd);
        } else {

        }
    }

    void GrabProxy::KeyboardEvent(KeyboardData & kd)
    {

    }

    /* Function:        GrabProxy::ButtonPressed
     * Description:     Called when a button is pressed.
     */
    /*void GrabProxy::ButtonPressed(int x, int y, BUTTON button, XScreen * screen)
    {
        // Set the state of the button.
        m_keyboard->KeyPressed(button);

        PointerData pd;
        pd.m_type           = PointerData::ButtonPressEvent;
        pd.m_screen         = screen;
        pd.Press.button     = button;
        pd.Press.state      = m_keyboard->GetModifierState() | m_pointer->GetButtonStates();
        pd.Press.ts         = m_server->ServerTime();
        pd.Press.x          = x;
        pd.Press.y          = y;

#ifdef DEBUG_VERBOSE
        cerr << "Button pressed at location: " << x << "," << y << " timestamp: " << pd.Press.ts << endl;
#endif
        if(m_info.isPointerGrabbed()) {
            HandleGrabbedPointerEvent(pd);
        } else {
            // The pointer is not grabbed. But this will either activate a passive grab
            // or generate a automatically grab.
        }
    }*/

    /* Function:        GrabProxy::ButtonReleased
     * Description:     Called when a button is released.
     */
    /*void GrabProxy::ButtonReleased(int x, int y, BUTTON button, XScreen * screen)
    {
        m_keyboard->KeyReleased(button);
        PointerData pd;
        pd.m_type           = PointerData::ButtonReleaseEvent;
        pd.m_screen         = screen;
        pd.Press.button     = button;
        pd.Press.state      = m_keyboard->GetModifierState() | m_pointer->GetButtonStates();
        pd.Press.ts         = m_server->ServerTime();
        pd.Press.x          = x;
        pd.Press.y          = y;

#ifdef DEBUG_VERBOSE
        cerr << "Button released at position: " << x << "," << y << " timestamp: " << pd.Press.ts << endl; 
#endif
        if(m_info.isPointerGrabbed()) {
            HandleGrabbedPointerEvent(pd);
        } else {
            // The pointer is not grabbed.
        }
    }*/

    /* Function:        GrabProxy::GrabKeyboard 
     * Description:     Grabs the keyboard
     */

    int GrabProxy::GrabKeyboard(const XClient * client, 
        BITMASK bitmask,
        bool ownerEvents, 
        bool keyboardFreeze, 
        bool pointerFreeze,
        TIMESTAMP timestamp)
    {
        return 0;
    }

    /* Function:        GrabProxy::GrabPointer
     * Description:     Grabs the pointer.
     */
    int GrabProxy::GrabPointer(XClient * client,    // the client requesting the grab
        WindowPtr grabWindow,                           // the grab window
        bool ownerEvents,                               // owner events
        BITMASK events,                                 // event mask
        int PointerMode,                                // pointer mode 
        int KeyboardMode,                               // keyboard mode.
        WindowPtr confineTo,
        CursorPtr cursor,
        TIMESTAMP ts)
    {
        if(m_info.isPointerGrabbed())
        {
            // the pointer is grabbed.
            if(m_info.PointerGrab.grabClient != client) {
                // not grabbed by this client, so ignore it.
            } else {
                // grabbed by this client.
                if(PointerMode == GrabInfo::Async) {
                }
            }
        } else {
            // the pointer is not grabbed.
            if(confineTo.valid()) {
                // confine the pointer to this window, will generate leave and enter 
                // events that will be sent before the grab is activated.
                m_server->ConfinePointerTo(confineTo);
            }
            m_info.PointerGrab.confineTo    = confineTo;
            m_info.PointerGrab.grabWindow   = grabWindow;
            m_info.PointerGrab.eventMask    = events;
            m_info.PointerGrab.grabClient   = client;
            m_info.PointerGrab.ownerEvents  = ownerEvents;
            m_info.PointerGrab.cursor       = cursor;
            
            if(PointerMode == GrabInfo::Async) {
                // the pointer events are reported as normal
                m_info.PointerGrab.mode = GrabInfo::Async;
            } else {
                // the pointer events are freezed.
                m_info.PointerGrab.mode = GrabInfo::Synch;
            }
            if(cursor.valid()) {
                // a cursor was supplied.
            }
        }
        return 0;
    }

    /* Function:        GrabProxy::GrabButton
     * Description:     Installs a passive grab for a button.
     */
    int GrabProxy::GrabButton(XClient * client, 
            DWORD modifiers, 
            BUTTON button, 
            XUtility::SmartPointer<XWindow> & grabWindow, 
            BOOL ownerEvents, 
            DWORD eventMask, 
            byte mode, 
            WINDOW confineTo,
            CURSOR cursor,
            ErrorStruct * error)
    {
        std::list<PassiveGrab> & grabs = grabWindow->GetGrabs();
        for(std::list<PassiveGrab>::iterator it = grabs.begin();
            it != grabs.end();
            it++)
        {
            if(it->m_type == PassiveGrab::ButtonGrab)
            {
                if(it->buttongrab.button == button && 
                    it->buttongrab.modifiers == modifiers)
                {
                    if(it->m_client != client) {
                        // fail with a a access error.
                        error->ErrorCode = ErrorStruct::AccessError;
                        return REQUEST_FAILED;
                    }
                    // override this with the new grab
                    it->buttongrab.confineTo    = confineTo;
                    it->buttongrab.cursor       = cursor;
                    it->buttongrab.eventMask    = eventMask;
                    it->buttongrab.pointerMode  = mode;
                    it->buttongrab.ownerEvents  = ownerEvents;
                    // the grab has been modified.
                    return REQUEST_SUCCESS;
                }
            }
        }
        // the client has not installed a previous grab that matches the button/key combination,
        // so install a new grab.
        PassiveGrab grab;
        grab.m_type = PassiveGrab::ButtonGrab;
        grab.buttongrab.button      = button;
        grab.buttongrab.confineTo   = confineTo;
        grab.buttongrab.cursor      = cursor;
        grab.buttongrab.ownerEvents = ownerEvents;
        grab.buttongrab.eventMask   = eventMask;
        grab.buttongrab.modifiers   = modifiers;
        grab.buttongrab.pointerMode = mode;
        
        grab.m_client = client;
        // add the grab to the list
        grabs.push_back(grab);
        return REQUEST_SUCCESS;
    }

    /* Function:        GrabProxy::ActivateButtonGrab
     * Description:     Activates a passive button grab.
     */
    bool GrabProxy::ActivateButtonGrab(X::ButtonGrabStruct * grab, XClient * client)
    {
        ErrorStruct error;
        if(m_info.isPointerGrabbed()) {
            cerr << "GrabProxy::ActivateButtonGrab: Can't activate the grab because the pointer is currently grabbed" << endl;
            return false;
        }

        WindowPtr grabWnd, confineTo;
        if(!m_server->XFindWindow(grab->grabWindow, grabWnd, &error)) {
            cerr << "GrabProxy::ActivateButtonGrab: Can't activate the passive grab because the grab window does not exist" << endl;
            return false;
        }

        if(grab->confineTo != 0)
        {
            if(!m_server->XFindWindow(grab->confineTo, confineTo, &error)) {
                cerr << "GrabProxy::ActivateButtonGrab: Can't activate the passive grab because the window to confine the pointer to does not exist" << endl;
                return false;
            }
        }

        CursorPtr cursor;
        // grab the pointer
        GrabPointer(client, grabWnd, grab->ownerEvents, grab->eventMask,grab->pointerMode, grab->keyboardMode, confineTo, cursor, m_server->ServerTime()); 

    }
};