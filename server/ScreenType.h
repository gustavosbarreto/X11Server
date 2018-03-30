#ifndef _SCREENTYPE_H_
#define _SCREENTYPE_H_

#include "types.h"
#include "DepthType.h"

namespace X
{
    struct ScreenType
    {
        enum BackingStore {None = 0, WhenMapped, Always};
        WINDOW root;
        uint16 width_pixels, height_pixels;     // the width and height in pixels
        uint16 width_mms, height_mms;           // the width and height in millimeters
        std::list<DepthType> listOfDepths;      // the allowed depths.
        byte rootDepth;
        VISUALID rootVisual;
        COLORMAP defaultColormap;
        uint32 whitePixel, blackPixel;
        uint16 minInstalledMaps, maxInstalledMaps;
        BackingStore backingStore;
        BOOL saveUnders;
        // TODO set of events.
    };
};

#endif