#ifndef _SERVERINFO_H_
#define _SERVERINFO_H_

#include "types.h"
#include <list>
#include "FormatType.h"
#include "ScreenType.h"

namespace X
{
    /* Struct:          ServerInfo
     * Description:     The server information that is sent to the client
     */
    struct ServerInfo
    {
        uint16 majorVersion, minorVersion;              // the major and minor version
        STRING8 vendor;                                 // the vendor string.
        uint32 releaseNumber;                           // the release number.
        uint32 resourceIdBase, resourceIdMask;
        byte imageByteOrder;
        byte bitmapScanlineUnit, bitmapScanlinePad;
        byte bitmapBitOrder;
        std::list<FormatType> pixmapFormats;
        std::list<ScreenType> roots;
        uint32 motionBufferSize;
        uint16 maxRequestLength;
        KEYCODE minKeycode, maxKeycode;
        uint32 vendorLength;
    };
};

#endif