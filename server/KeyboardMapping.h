#ifndef _KEYBOARDMAPPING_H_
#define _KEYBOARDMAPPING_H_

#include "XDefines.h"

namespace X
{
    /* Struct:          KeyboardMapping
     * Description:     Represents the mapping table.
     */
    struct KeyboardMapping
    {
        size_t minKeycode, maxKeycode, keysymsPerKeycode;
        KEYSYM * m_table;
        KEYSYM modifiers[8][4];     // contains the actual keysyms mapped to modifiers 
    };
};

#endif