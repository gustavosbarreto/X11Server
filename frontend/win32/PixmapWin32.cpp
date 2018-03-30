#include "PixmapWin32.h"
#include <iostream>

using namespace std;

namespace XWin32
{
    /* Function:        PixmapWin32::PixmapWin32
     * Description:     Constructor.
     */
    PixmapWin32::PixmapWin32()
    {
        m_hDC = 0;
        m_hBitmap = 0;
    }

    /* Function:        PixmapWin32::~PixmapWin32
     * Description:     Destructor.
     */
    PixmapWin32::~PixmapWin32()
    {
        if(m_hBitmap)
            DeleteObject(m_hBitmap);
    }

    /* Function:        XWin32::Init
     * Description:     Initalizes the pixmap.
     */
    int PixmapWin32::Init(HWND root,const X::PixelFormat * format, int w, int h)
    {
        m_bits = new unsigned char[w * h * format->bitsPerPixel];
        if(m_bits == NULL) {
            cerr << "Could not allocate the temporary buffer" << endl;
            return -1;
        }

        m_hDC = CreateCompatibleDC(GetDC(root));

        // Create the bitmap
        memset(&m_infohdr, 0, sizeof(m_infohdr));
        m_infohdr.bV5Size           = sizeof(m_infohdr);
        m_infohdr.bV5Width          = w;
        m_infohdr.bV5Height         = -h;   // top down bitmap
        m_infohdr.bV5Planes         = 1;
        m_infohdr.bV5BitCount       = format->depth;
        m_infohdr.bV5Compression    = BI_RGB;
        m_infohdr.bV5SizeImage      = 0;
    
        m_hBitmap = CreateDIBitmap(m_hDC, (BITMAPINFOHEADER *)&m_infohdr, 0,0,0, DIB_RGB_COLORS);
        if(m_hBitmap == 0) {
            cerr << "CreateDIBitmap() failed" << endl;
            return -1;
        } else {
            SelectObject(m_hDC, m_hBitmap);
            return 0;
        }
    }

    /* Function:        PixmapWin32::PutZImage
     *
     */
    int PixmapWin32::PutZImage(     
        int x, 
        int y, 
        int width, 
        int height, 
        const unsigned char *src)
    {
        if(x >= m_width || y >= m_height) 
            return -1;
        // fill out the bitmap information.
        BITMAPINFO info;
        memset(&info, 0, sizeof(info));
        info.bmiHeader.biBitCount   = m_depth;
        info.bmiHeader.biHeight     = min(height, m_height - y);
        info.bmiHeader.biWidth      = min(width, m_width - x);
        info.bmiHeader.biPlanes     = 1;
        // blit the actual image
        StretchDIBits(m_hDC, x, y, info.bmiHeader.biWidth, info.bmiHeader.biHeight,0,0,width,height,src,&info, DIB_RGB_COLORS, 0);
        return 0;
    }

    /* Function:        PixmapWin32::GetZImage
     *
     */
    int PixmapWin32::GetZImage(int x, int y, int w, int h, unsigned char * dst)
    {
        BITMAPINFO info;
        if(x >= m_width || y >= m_height || w > m_width || h > m_width)
            return -1;

        if(x == 0 && w == m_width) {
            GetDIBits(m_hDC, m_hBitmap, y, h, dst, &info, 0);
            return 0;
        } else {
            void * data;
            memset(&info, 0, sizeof(info));
            info.bmiHeader.biSize = sizeof(info);
            info.bmiHeader.biHeight = h;
            info.bmiHeader.biWidth  = w;
            info.bmiHeader.biBitCount = m_depth;
            // create the DIB section.
            HDC hDst = CreateCompatibleDC(m_hDC);
            if(hDst == 0) {
                cerr << "CreateCompatibleDC() failed" << endl;
                return -1;
            }
            HBITMAP bitmap = CreateDIBSection(hDst, &info, DIB_RGB_COLORS, &data, NULL, NULL);
            if(bitmap == 0) {
                cerr << "Could not create the DIB section" << endl;
                DeleteDC(hDst);
                return -1;
            }
            SelectObject(hDst, bitmap);
            BitBlt(hDst, 0, 0, w, h, m_hDC, x, y, SRCCOPY);
            // now copy the actual data.
            memcpy(dst, data, w * h * (m_depth / 8));
            // now free the DC and bitmap
            SelectObject(hDst, 0);
            DeleteObject(bitmap);
            DeleteDC(hDst);
            return 0;
        }
    }

    /* Function:        PixmapWin32::TileArea
     * Description:     Tiles the area with the supplied pixmap.
     */
    bool PixmapWin32::TileArea(const X::XRectangle & rect, XUtility::SmartPointer<X::PixmapImpl> pixmap)
    {
        XUtility::SmartPointer<PixmapWin32> _pixmap = (PixmapWin32 *) *pixmap;
        int cx, cy;
        for(int nx = rect.m_x, dx = rect.m_x + rect.m_width;nx < dx; nx += _pixmap->m_width)
        {
            for(int ny = rect.m_y, dy = rect.m_y + rect.m_height; ny < dy; ny += _pixmap->m_height)
            {
                cx = min(_pixmap->m_width, dx - nx);
                cy = min(_pixmap->m_height, dy - ny);
                if(!BitBlt(m_hDC, nx, ny, cx, cy, _pixmap->m_hDC, 0, 0, SRCCOPY))
                {
                    cerr << "BitBlt() failed when tileing area" << endl;
                    return false;
                }
            }
        }
        return true;
    }

    /* Function:        PixmapWin32::PutPixmap
     * Description:     Puts a pixmap.
     */
    bool PixmapWin32::PutPixmap(XUtility::SmartPointer<PixmapImpl> & pixmap, const X::XRectangle & rect)
    {
        PixmapWin32 * src = (PixmapWin32 *) *pixmap;
        if(rect.m_x >= m_width || rect.m_y >= m_height) return true;
        // blit the source onto the destination.
        return BitBlt(m_hDC, rect.m_x, rect.m_y, 
            min(rect.m_width, m_width - rect.m_x),
            min(rect.m_height, m_height - rect.m_y),
            src->m_hDC, 0, 0, SRCCOPY);
    }

    /* Function:        PixmapWin32::Fill
     * Description:
     */
    void PixmapWin32::Fill(X::PIXEL pixel, const X::XRectangle * xrect)
    {
        // extract the components from the pixel
        int r, g, b;
        PixelToComponents(pixel, r,g,b);
        HBRUSH brush    = CreateSolidBrush(RGB(r, g, b));
        RECT rect;
        if(xrect) {
            rect.left = xrect->m_x;
            rect.top = xrect->m_y;
            rect.bottom = xrect->m_height + xrect->m_y;
            rect.right = xrect->m_x + xrect->m_width;
        } else {
            // fill the entire pixmap
            rect.left = rect.top = 0;
            rect.bottom = m_height;
            rect.right = m_width;
        }
        FillRect(m_hDC, &rect, brush);
        DeleteObject(brush);
    }

    /*
     *
     */
    void PixmapWin32::PixelToComponents(X::PIXEL pixel, int & r, int & g, int & b) 
    {
        switch(m_depth)
        {
        case 16:
            b = pixel & 0x1F;
            g = (pixel >> 5) & 0x1F;
            r = (pixel >> 10) & 0x1F;
            break;
        case 24:
        case 32:
            b = pixel & 0xFF;
            g = (pixel >> 8) & 0xFF;
            r = (pixel >> 16) & 0xFF;
            break;
        }
    }
};