#ifndef _POINTERDATA_H_
#define _POINTERDATA_H_

#include "types.h"

namespace X
{
    class XScreen;
    /*
     *
     */
    struct PointerData
    {
        enum type {ButtonPressEvent = 0, ButtonReleaseEvent, PointerMoveEvent};
        union
        {
            struct ButtonPress
            {
                BUTTON      button;     // the pressed button
                int         x, y;       // the x and y position of the pointer
                TIMESTAMP   ts;         // the time 
                BITMASK     state;      // the state of the buttons and modifier keys.
                bool AllButtonsReleased() const {return !(state & (0xF00));}
            } Press;

            struct PointerMove
            {
                int         x, y;
                TIMESTAMP   ts;
                BITMASK     state;
            } Movement;
        };
        XScreen * m_screen;
        type m_type;
    };
};

#endif