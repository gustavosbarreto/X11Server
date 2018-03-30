#ifndef _IPOINTERNOTIFY_H_
#define _IPOINTERNOTIFY_H_

#include "XEvent.h"

namespace X
{
    /* Class:           IPointerNotify
     * Description:     
     */
    class IPointerNotify
    {
    public:
        virtual void OnPointerEvent(const XEvent &) = 0;
    };
};

#endif