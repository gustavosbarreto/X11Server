#ifndef _GLRENDERER_H_
#define _GLRENDERER_H_

#include "XDrawable.h"

namespace X
{
    class GLRenderer
    {
    public:
        static bool TileArea(IRenderSurface *, int, int, int,int);
        static bool Draw(IRenderSurface *, const XRectangle & dst);
        static bool Fill(XRectangle, float r, float g, float b);
        static bool Clear();
        static void SwapBuffers(IRenderTarget *);
        static void Ortho2d(int left, int right, int bottom, int top);
        static void LoadIdentity();
        static void DrawTexturedRect(int,int,int,int);
    };
};

#endif