#ifndef _DEPTHTYPE_H_
#define _DEPTHTYPE_H_

#include <list>
#include "VisualType.h"

namespace X
{
    /* Struct:          DepthType
     * Description: 
     */
    struct DepthType
    {
        byte depth;
        std::list<VisualType> visuals;
    };

    typedef std::list<DepthType> DepthList;
};

#endif