#ifndef _PIXELFORMAT_H_
#define _PIXELFORMAT_H_

namespace X
{
    /* Struct:          PixelFormat
     * Description:     
     */
    struct PixelFormat
    {
        int depth, 
            rmask, gmask, bmask,amask,
            pitch,
            bitsPerPixel;
        int RedBits, BlueBits, GreenBits, AlphaBits;
        HDC m_hDC;
        int m_depth;
    };
};

#endif