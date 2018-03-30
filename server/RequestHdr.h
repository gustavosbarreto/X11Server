#ifndef _REQUESTHDR_H_
#define _REQUESTHDR_H_

namespace X
{
    /* Struct:          RequestHdr
     * Description:     The request header
     */
#pragma pack(push, 1)
    struct RequestHdr
    {
        byte    opcode;
        uint16  size;       // expressed in multiples of four.
        byte    data;
    };
#pragma pack(pop)
};

#endif