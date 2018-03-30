#ifndef _XEVENT_H_
#define _XEVENT_H_

#include "types.h"
#include "Events.h"

namespace X
{
    class XEvent
    {
    public:
        enum EventType {KeyPressed = 0, KeyReleased, ButtonPressed, ButtonReleased,
            MotionNotify, EnterNotify, LeaveNotify, FocusIn, FocusOut, MapNotify, UnmapNotify, MapRequest,
            DestroyNotify, VisibilityNotify
        };

        union 
        {
            PointerEvent            pointerEvent;
            CrossingEvent           crossingEvent;
            MapEvent                mapNotify;
            MapEvent                unmapNotify;
            MapRequestEvent         mapRequest;
            DestroyNotifyEvent      destroyNotify;
            VisibilityNotifyEvent   visibilityNotify;
                
            //ExposeEvent               exposeEvent;
            /*GraphicsExposureEvent graphicsExposure;
            NoExposureEvent         noExposure;
            VisibilityNotifyEvent   visibilityNotify;
            CreateNotifyEvent       createNotify;
            DestroyNotifyEvent      destroyNotify;
            UnmapNotifyEvent        unmapNotify;
            MapNotifyEvent          mapNotify;
            MapRequestEvent         mapRequest;
            ReparentNotifyEvent     reparentNotify;
            ConfigureNotifyEvent    configureNotify;
            ConfigureRequestEvent   configureRequest;
            GravityNotifyEvent      gravityNotify;
            ResizeRequestEvent      resizeRequest;
            CirculateNotifyEvent    circulateNotify;
            */
            /* Todo: more events */

        };
        bool isMaskedEvent(BITMASK) const;
        EventType m_type;
    };
};


#endif