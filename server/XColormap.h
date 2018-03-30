#ifndef _XCOLORMAP_H_
#define _XCOLORMAP_H_

#include "types.h"
#include "XResource.h"
#include "VisualType.h"

namespace X
{
    /* Class:           XColormap
     * Description:     Represents a colormap
     */
    class XColormap : public XResource
    {
    public:


    protected:
        VisualType & m_type;        // the visual type
        PIXEL * m_values;           // the pixel values.
    };
};


#endif