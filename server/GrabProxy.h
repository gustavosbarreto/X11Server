#ifndef _GRABPROXY_H_
#define _GRABPROXY_H_

#include "types.h"
#include <list>
#include "XClient.h"
#include "IKeyboardNotify.h"
#include "IPointerNotify.h"
#include "SmartPointer.h"
#include "ErrorStruct.h"
#include <queue>
#include "GrabInfo.h"
#include "PointerData.h"
#include "XKeyboard.h"
#include "XPointer.h"
#include "PassiveGrab.h"
#include "KeyboardData.h"

namespace X
{
    class XWindow;
    class XClient;
    /* Function:        GrabProxy
     * Description:     Proxy for the keyboard and pointer events. Will make sure
     *                  that the events is only sent to the right client(s). 
     */
    class GrabProxy
    {
    public:
        // Constructor.
        GrabProxy(XServer *, XKeyboard *, XPointer *);
        
        // Called with the raw data about the pointer and keyboard. May queue the data if
        // the cursor or pointer is currently freezed.

        void PointerEvent(PointerData &);
        void KeyboardEvent(KeyboardData &);

        /*void PointerMoved(int x, int y, XScreen *);
        void ButtonPressed(int x, int y, BUTTON, XScreen *);
        void ButtonReleased(int x, int y, BUTTON, XScreen *);

        void KeyPressed(KEYCODE, int x, int y, XScreen *);
        void KeyReleased(KEYCODE, int x, int y, XScreen *);
        */
        int AllowEvents(int mode, TIMESTAMP ts, XClient * client, ErrorStruct *);


        // Handles translated events.
        //void OnKeyboardEvent(int x, int y, X::XScreen * screen);
        //void OnPointerEvent(int x, int y, BUTTON button, XScreen * screen);

        // Grabs the keyboard/pointer.
        int GrabKeyboard(const XClient *, BITMASK,bool ownerEvents, bool keyboardFreeze, bool pointerFreeze, TIMESTAMP);
        int GrabPointer(XClient *,WindowPtr,bool ownerEvents, BITMASK, int,int,WindowPtr, CursorPtr, TIMESTAMP);
        
        // sets a passive grab on a button
        int GrabButton(XClient *, DWORD, BUTTON, XUtility::SmartPointer<XWindow> &, BOOL, DWORD, byte, WINDOW, CURSOR, ErrorStruct *);
    private:

        // processes the queued pointer events.
        void ProcessQueuedPointerEvents();
        void HandlePointerEvent(const PointerData & pd);
        void HandleGrabbedPointerEvent(const PointerData & pd);

        void OnPointerMove(const PointerData & pd);
        void OnButtonPress(const PointerData & pd);
        void OnButtonRelease(const PointerData & pd);

        bool ActivateButtonGrab(ButtonGrabStruct *, XClient *);

        GrabInfo m_info;        // the information about the grab.
        std::queue<PointerData> m_pointerQueue;

        XKeyboard   * m_keyboard;
        XPointer    * m_pointer;
        XServer     * m_server;
        WindowPtr   m_oldWnd;
    };
};

#endif