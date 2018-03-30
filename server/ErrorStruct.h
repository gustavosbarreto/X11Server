#ifndef _ERRORSTRUCT_H_
#define _ERRORSTRUCT_H_

#include "types.h"

namespace X
{
    /* Function:        ErrorStruct
     * Description:     Contains error information that will be sent to the client.
     */
    struct ErrorStruct
    {
    public:
        enum Error {RequestError = 1, ValueError, WindowError, PixmapError, AtomError, CursorError, FontError,
            MatchError, DrawableError, AccessError, AllocError, ColormapError, GContextError, IDChoiceError,
            NameError, LengthError, ImplementationError};

        byte ErrorCode;
        uint16 SequenceNumber;
        uint32 Value;
        uint16 minor;
        byte major;
    };
};

#endif