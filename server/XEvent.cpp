#include "XEvent.h"

namespace X
{
    /* Function:        XEvent::isMaskedEvent
     * Description:     Returns true if the event is a masked one.
     */
    bool XEvent::isMaskedEvent(BITMASK bitmask) const
    {
        return static_cast<bool>(m_type & bitmask);
    }

};