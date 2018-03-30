#ifndef _PASSIVEGRAB_H_
#define _PASSIVEGRAB_H_

#include "types.h"
/*
 *
 */

class XClient;

namespace X
{
    

    /* Struct:          ButtonGrab
     * Description: 
     */
    struct ButtonGrabStruct
    {
        SETofKEYMASK        modifiers;
        BUTTON              button;
        WINDOW              grabWindow;
        BOOL                ownerEvents;
        SETofPOINTEREVENT   eventMask;
        byte                pointerMode, keyboardMode;
        WINDOW              confineTo;
        CURSOR              cursor;
    };

    /* Struct:      KeyGrab
     *
     */

    struct KeyGrabStruct
    {
        KEYCODE         key;
        SETofKEYMASK    modifiers;
        WINDOW          grabWindow;
        BOOL            ownerEvents;
        byte            pointerMode, keyboardMode;
    };

    /*
     *
     */
    struct PassiveGrab
    {
        enum GrabType {KeyGrab = 0, ButtonGrab = 1};
        union
        {
            KeyGrabStruct       keygrab;
            ButtonGrabStruct    buttongrab;
        };

        GrabType m_type;
        // The client who set the grab.
        XClient * m_client;
    };
};

#endif