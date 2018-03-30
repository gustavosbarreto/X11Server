#ifndef _XFRONT_H_
#define _XFRONT_H_

#include "XServer.h"

namespace X
{
    /* Class:           XFront
     * Description:     The front end of the X server.
     */
    class XFront
    {
    public:

    protected:
        XServer *   m_server;       // the backend, the actual X implementation.
    };
};

#endif