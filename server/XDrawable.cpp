#include "XDrawable.h"
#include <iostream>
#include <assert.h>
#include <gl/gl.h>
#include "XServer.h"

using namespace std;

namespace X
{
    /* Function:        XDrawable::XDrawable
     * Description:     Constructor.
     */
    XDrawable::XDrawable(X::uint32 id , X::XScreen * screen, XServer * server) 
        : XResource(id), m_screen(screen), m_server(server)
    {
        m_surface = NULL;
    }

    /* Function:        XDrawable::~XDrawable
     * Description:     Performs the required cleanup
     */
    XDrawable::~XDrawable()
    {
        delete m_surface;
    }

    /* Function:        XDrawable::AllocSurface
     * Description:     
     */
    bool XDrawable::AllocSurface(unsigned int w, unsigned int h)
    {
        if(!(m_surface = m_server->CreateRenderSurface(w, h)))
        {
            cerr << "XDrawable::AllocSurface: Server failed to allocate the offscreen rendering surface" << endl;
            return false;
        }
        return true;
    }

    /*
     *
     */
    bool XDrawable::BindTarget()
    {
        if(m_surface == NULL)
            return false;
        return m_surface->BindTarget();
    }
    /*
     *
     */
    int XDrawable::PutBitmapImage(
        const XGContext * context, int x,int y, byte depth, int w, int h, byte pad, byte * src, size_t srclen)
    {
        // make sure that we don't read outside the array
        if((w * h) > (srclen * 8)) {
            return -1;
        }
        assert(false);      // not implemented yet.
        return 0;
    }

    /* Function:        XDrawable::PutZImage
     * Description:     Draws a Z image to the drawable.
     */
    int XDrawable::PutZImage(
        const XGContext * context, int x,int y,int w,int h,const byte * data, size_t length)
    {
        // bind the rendering surface as the render target
        if(m_surface == NULL) {
            cerr << "XDrawable::PutZImage: No valid surface, no drawing is allowed" << endl;
            return -1;
        }
        if(!m_surface->BindTarget()) {
            cerr << "XDrawable::PutZImage: Faild to bind the surface as the rendering target" << endl;
            return -1;
        }

        switch(m_depth)
        {
        case 24:
            // No conversion required, just draw the pixels.
            glRasterPos2i(x, y);
            glDrawPixels(w, h, GL_RGB, GL_UNSIGNED_BYTE, data);
            break;
        case 16:
            {
                // the data is stored as RGB with 5 bits each.
                float * pixels = new (std::nothrow) float[w * h * 3]; // RGB , 3 floats per pixel
                float * dst = pixels;
                unsigned short * src = (unsigned short *) data;
                const float m = 1.0f / 32.0f;
                for(size_t i = 0;i<(w*h);++i) 
                {
                    unsigned short pixel = src[i];
                     *(dst++) = float(pixel >> 11) * m;             // the red component
                     *(dst++) = float((pixel >> 6) & 0x1F) * m;     // the green component
                     *(dst++) = float((pixel >> 1) & 0x1F) * m;     // the blue component
                    // all the pixels has been converted.
                }
                glRasterPos2i(x, y);
                glDrawPixels(w, h, GL_RGB, GL_FLOAT, pixels);
                // now delete the allocated memory
                delete [] pixels;
                break;
            }
        case 8:
            {
                // the pixels are stored as RGB with 3 bits for red, 3 bits for green and 2 bits for blue
                float * pixels = new (std::nothrow) float[w * h * 3]; // RGB , 3 floats per pixel
                float * dst = pixels;
                const float m1 = 1.0f / 8.0f, m2 = 0.25f; // 1.0f / 4.0f
                for(size_t i = 0;i<(w*h);++i) 
                {
                    byte pixel = data[i];
                     *(dst++) = float(pixel >> 5)           * m1;       // the red component
                     *(dst++) = float((pixel >> 2) & 0x07)  * m1;       // the green component
                     *(dst++) = float((pixel) & 0x03)       * m2;       // the blue component
                }
                // all the pixels has been converted.
                glRasterPos2i(x, y);
                glDrawPixels(w, h, GL_RGB, GL_FLOAT, pixels);
                // now delete the allocated memory
                delete [] pixels;
                break;
            }
        }
        // unbind the pixel buffer
        m_surface->UnbindTarget();
        return 0;
    }

    /*
     *
     */
    int XDrawable::GetZImage(int x,int y,int w,int h, BITMASK mask, byte * dst, size_t & len)
    {
        // bind the pixel buffer
        switch(m_depth)
        {
        case 24:
            if(len < (w * h * 3)) {
                cerr << "The supplied buffer is too short to hold the image" << endl;
                return -1;
            }
            glReadPixels(x, y, w, h, GL_RGB, GL_UNSIGNED_BYTE, dst);
            len = (w*h*3);
            if((mask & 0x00FFFFFF) != 0xFFFFFF) {
                // need to zero some bits
            }
            break;
        case 16:
            {
                // store the pixels as r(5)g(5)b(5)
                if(len < (w*h*2)) {
                    cerr << "The supplied buffer is too short to hold the image" << endl;
                    return -1;
                }
                // allocate a buffer to store the pixel data in
                unsigned char * buff = new (std::nothrow) unsigned char[w*h*3];
                unsigned char * current = buff;
                unsigned short * dstw = (unsigned short *) dst;
                // read the pixels.
                glReadPixels(x, y, w, h, GL_RGB, GL_UNSIGNED_BYTE, buff);
                // now convert the pixels to the required format
                byte r,g,b;
                unsigned short pixel;

                if((mask & 0xFFFF) != 0xFFFF) // does he want all the bits of a pixel?
                {
                    // need to mask the pixels.
                    for(size_t i = 0;i<(w*h);++i)
                    {
                        // simple solution for now, just ignore the lower 3 bits
                        r = *(current++) & 0xF8; g = *(current++) & 0xF8; b = *(current++) & 0xF8;
                        pixel = r << 11 | g << 6 | b << 1;
                        // now write the pixel to the destination
                        *(dstw++) = pixel & mask;
                    }
                } else
                {
                    for(size_t i = 0;i<(w*h);++i)
                    {
                        // simple solution for now, just ignore the lower 3 bits
                        r = *(current++) & 0xF8; g = *(current++) & 0xF8; b = *(current++) & 0xF8;
                        pixel = r << 11 | g << 6 | b << 1;
                        // now write the pixel to the destination
                        *(dstw++) = pixel;
                    }
                }
                delete [] buff;
                break;
            }
        case 8:
            {
                // store the image as r(3)g(3)b(2)
                assert(false);      // not implemented yet.
                wglMakeCurrent(NULL, NULL);
                return -1;
            }
        default:
            cerr << "Invalid pixel depth" << endl;
            return -1;
        }
        // unbind the pixel buffer.
        return 0;
    }
};