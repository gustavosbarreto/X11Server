#ifndef _FORMATTYPE_H_
#define _FORMATTYPE_H_

namespace X
{
    struct FormatType
    {
        byte depth;
        byte bitsPerPixel;  // 1,4,8,16,24,32
        byte scanlinePadd;  // 8, 16, 32
    };
};

#endif