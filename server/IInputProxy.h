#ifndef _IINPUTPROXY_H_
#define _IINPUTPROXY_H_

// The modifier keys.
#define X_LEFT_SHIFT_MODIFIER       (1 << 0)
#define X_RIGHT_SHIFT_MODIFIER      (1 << 1)
#define X_LEFT_CTRL_MODIFIER        (1 << 2)
#define X_RIGHT_CTRL_MODIFIER       (1 << 3)
#define X_ALT_MODIFIER              (1 << 4)
#define X_ALT_GR_MODIFIER           (1 << 5)

// Mouse buttons
#define X_LEFT_BUTTON_PRESSED       (1 << 6)
#define X_RIGHT_BUTTON_PRESSED      (1 << 7)
#define X_MIDDLE_BUTTON_PRESSED     (1 << 8)

#include "PointerData.h"

namespace X
{
    /* Function:        IInputProxy
     * Description:     Input proxy interface class.
     */
    class IInputProxy
    {
    public:
        virtual void MouseMoved(int x,int y, unsigned int mask) = 0;
        virtual void ButtonPressed(int x,int y, BUTTON button, unsigned int mask) = 0;

    };
};

#endif