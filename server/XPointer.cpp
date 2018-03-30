#include "XPointer.h"

namespace X
{
    XPointer::XPointer()
    {
        m_buttons = 0;
    }

    /*
     *
     */
    bool XPointer::IsButtonDown(ButtonId button)
    {
        return m_buttons & (1 << button);
    }

    /* Function:        XPointer::ButtonPressed
     * Description:     Sets the current state of a button to pressed.
     */
    void XPointer::ButtonPressed(ButtonId button)
    {
        m_buttons |= (1 << button);
    }

    /* Function:        XPointer::ButtonReleased
     * Description:     Sets the state of a button to released.
     */
    void XPointer::ButtonReleased(X::ButtonId button)
    {
        m_buttons &= ~(1 << button);
    }

    /*
     *
     */
    SETofKEYBUTMASK XPointer::GetButtonStates()
    {
        return (m_buttons << 8);        // need to shift this so it will match the
                                        // SETofKEYBUTMASK format.
    }
};