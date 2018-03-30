#ifndef _VISUALTYPE_H_
#define _VISUALTYPE_H_

namespace X
{
    /* Struct:          VisualType
     *
     */
    struct VisualType
    {
        VISUALID    id; 
        // the different visual classes.
        enum VisualClass {StaticGray, StaticColor, TrueColor, GrayScale, PseudoColor, DirectColor};
        VisualClass     type_class;
        uint32      red_mask, green_mask, blue_mask;
        byte        bits_per_rgb_value;
        uint32      colourmap_entries;
    };
};

#endif