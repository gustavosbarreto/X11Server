#include "PixmapSDL.h"

namespace SDLX
{
    /* Function:        PixmapSDL::init
     * Description:     Initalizes the pixmap.
     */
    int PixmapSDL::Init(int w, int h, char * pixels, const X::PixelFormat * format)
    {
        pixmap = SDL_CreateRGBSurfaceFrom(pixels, w, h, format->depth,format->pitch,
            format->rmask, format->gmask, format->bmask, format->amask);
        if(pixmap == NULL)
            return -1;
        return 0;
    }

    /* Function:        PixmapSDL::Blit
     * Description:     Blits the surface to another one.
     */
    int PixmapSDL::Blit(const PixmapSDL * src, int x, int y, const X::XRectangle & rect)
    {
        if((x + rect.m_width)  > m_w || (y + rect.m_height) > m_h)
            return -1;

        SDL_Rect srcrect = {rect.m_x, rect.m_y, rect.m_width, rect.m_height},
            dstrect = {x, y, rect.m_width, rect.m_height};
        return SDL_BlitSurface(src->pixmap, &srcrect, pixmap, &dstrect); 
    }
};