#ifndef _REQUESTSTRUCTS_H_
#define _REQUESTSTRUCTS_H_

#include "types.h"

namespace X
{
    struct CreateWindowStruct
    {
        WINDOW wId, parentId;
        int16 x, y;
        uint16 w, h, borderWidth, windowClass;
        VISUALID visual;
        BITMASK mask;
        LISTofVALUE values;
    };
};

#endif