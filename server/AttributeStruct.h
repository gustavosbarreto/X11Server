#ifndef _ATTRIBUTESTRUCT_H_
#define _ATTRIBUTESTRUCT_H_

#include "types.h"

namespace X
{
    /* Struct:      AttributeStruct
     *
     */
    struct AttributeStruct
    {
        bool BackingStore;
        VISUALID visual;
        uint16 WindowClass;
        byte BitGravity;
        byte WinGravity;
        uint32 BackingPlanes;
        uint32 BackingPixel;
        bool SaveUnder;
        bool MapIsInstalled;
        byte MapState;
        bool OverrideRedirect;
        uint32 ColorMap;
        SETofEVENT AllEventMasks, yourEventMasks;
        SETofDEVICEEVENT doNotPropagateMask;
    };
};

#endif