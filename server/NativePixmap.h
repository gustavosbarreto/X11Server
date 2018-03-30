#ifndef _NATIVEPIXMAP_H_
#define _NATIVEPIXMAP_H_

#include "PixelFormat.h"
#include "SmartPointer.h"
#include "XPixmap.h"

namespace X
{
    /* Class:           NativePixmap
     * Description:
     */
    class NativePixmap
    {
    public:
        virtual int Init(int w, int h, char *, const X::PixelFormat *) = 0;
        virtual int Blit(const XUtility::SmartPointer<XPixmap> &) = 0;

    };
};

#endif