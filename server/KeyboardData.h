#ifndef _KEYBOARDDATA_H_
#define _KEYBOARDDATA_H_

#include "types.h"

namespace X
{
    /* Struct:      KeyboardData
     *
     */
    struct KeyboardData
    {
        enum Type {KeyPressed = 0, KeyReleased};
        byte        keycode;        
        TIMESTAMP   ts;
        BITMASK     mask;
        Type        m_type;
    };
};

#endif