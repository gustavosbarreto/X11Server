#ifndef _FRONTSDL_H_
#define _FRONTSDL_H_

#include "XFront.h"

namespace SDLX
{
    /* Class:           FrontSDL
     * Description:     Frontend for the X server using SDL.
     */
    class FrontSDL : public X::XFront
    {
    public:

    protected:
        // Draws a window.
        int DrawWindow(XUtility::SmartPointer<X::XWindow> &);

        // draws a pixmap
        int DrawPixmap(int x, int y, XUtility::SmartPointer<X::PixmapImpl> &);
    };
};

#endif