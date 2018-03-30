#ifndef _PIXMAPSDL_H_
#define _PIXMAPSDL_H_

#include "NativePixmap.h"
#include "SDL.h"
#include "XRectangle.h"

namespace SDLX
{
    /* Function:        PixmapSDL
     *
     */
    class PixmapSDL : public X::NativePixmap
    {
    public:
        PixmapSDL();
        int Init(int w, int h, char *, const X::PixelFormat *);
        int Blit(const PixmapSDL *, int x, int y, const X::XRectangle &);
    protected:
        SDL_Surface * pixmap;
        int m_x, m_y, m_h, m_w;     // the dimensions of the pixmap.

    };
};

#endif