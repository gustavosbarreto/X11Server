#ifndef _GRABINFO_H_
#define _GRABINFO_H_

#include "types.h"
#include "XWindow.h"
#include "XCursor.h"

namespace X
{
    /* Struct:          GrabInfo
     * Description: 
     */
    struct GrabInfo
    {   
        GrabInfo() 
        {
            PointerGrab.grabWindow = 0;
            PointerGrab.ownerEvents = false;
            PointerGrab.eventMask = 0;
            PointerGrab.mode = Synch;
            PointerGrab.grabClient = NULL;
            PointerGrab.confineTo = 0;
            PointerGrab.lastGrab = 0;
            PointerGrab.cursor = 0;
            //PointerGrab = {0, false, Synch, 0, 0,0,0, NULL};
            //KeyboardGrab = {0, false, Synch, 0, NULL};
        }

        enum Mode {Synch = 0, Async, AutoGrab, Async_UntilButtonEvent, Async_UntilKeyEvent,Async_UntilDeviceEvent};
        enum Reason {Passive = 0, GrabRequest};

        struct Pointer
        {
            WindowPtr   grabWindow;
            bool        ownerEvents;
            BITMASK     eventMask;
            Mode        mode;
            WindowPtr   confineTo;
            CursorPtr   cursor;
            TIMESTAMP   lastGrab;
            XClient *   grabClient;     // the client that grabbed the pointer
            XClient *   freezeClient;   // the client that freezed the pointer.
            Reason      reason;         // the reason the device was grabbed
        } PointerGrab;

        struct KeyBoard
        {
            WindowPtr   grabWindow;
            bool        ownerEvents;
            Mode        mode;
            TIMESTAMP   lastGrab;
            XClient *   grabClient;
            XClient *   freezeClient;   // the client that freezed the pointer.
            Reason      reason;         // the reason the device was grabbed
        } KeyboardGrab;

        bool isPointerGrabbed()     {return PointerGrab.grabClient != NULL;}
        bool isKeyboardGrabbed()    {return KeyboardGrab.grabClient != NULL;}
        bool isPointerFrozen()      {return PointerGrab.mode == Synch;}
        bool isKeyboardFrozen()     {return KeyboardGrab.mode == Synch;}
    };
};

#endif