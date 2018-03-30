#ifndef _PIXMAPWIN32_H_
#define _PIXMAPWIN32_H_

#include "PixelFormat.h"
#include "PixmapImpl.h"
#include "types.h"
#include "SmartPointer.h"
#include "Win32GC.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace XWin32
{
    /* Class:           PixmapWin32
     * Description:     Win32 implementation of pixmaps.
     */
    class PixmapWin32 : public X::PixmapImpl
    {
    public:
        PixmapWin32();
        ~PixmapWin32();

        int Init(HWND, const X::PixelFormat *, int, int);

        // draws a line.
        //int Line(XUtility::SmartPointer<X::XGContext>,int,int,int,int);

        // puts a Z image.
        int PutZImage(int,int,int,int,const unsigned char * src);
        int GetZImage(int,int,int,int,unsigned char *);
        bool TileArea(const X::XRectangle &, XUtility::SmartPointer<X::PixmapImpl> pixmap);
        bool PutPixmap(XUtility::SmartPointer<PixmapImpl> &, const X::XRectangle & rect);
        void Fill(X::PIXEL, const X::XRectangle * rect = NULL);
        void PixelToComponents(X::PIXEL, int &, int &, int &);

    protected:
        HDC                 m_hDC;
        HBITMAP             m_hBitmap;      // handle to the actual bitmap
        BITMAPV5HEADER      m_infohdr;
        unsigned char *     m_bits;         // used when writing a image.
        int                 m_width, m_height, m_depth;
    };
};

#endif