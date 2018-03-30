#ifndef _XPOINTER_H_
#define _XPOINTER_H_

#include "types.h"

namespace X
{
    enum ButtonId {Button1 = 0, Button2, Button3, Button4, Button5};
    
    /* Function:        XPointer
     * Description:     Represents the pointer.
     */
    class XPointer
    {
    public:
        XPointer();
        bool IsButtonDown(ButtonId);
        void ButtonPressed(ButtonId);
        void ButtonReleased(ButtonId);
        SETofKEYBUTMASK GetButtonStates();
    protected:
        SETofKEYBUTMASK m_buttons;
    };
};

#endif