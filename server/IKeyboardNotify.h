#ifndef _IKEYBOARDNOTIFY_H_
#define _IKEYBOARDNOTIFY_H_

#include "XEvent.h"

namespace X
{
    /*
     *
     */
    class IKeyboardNotify
    {
    public:
        virtual void OnKeyboardEvent(const XEvent &) = 0;
    };
};

#endif