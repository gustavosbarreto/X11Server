#ifndef _ARRAYSTREAM_H_
#define _ARRAYSTREAM_H_

#include "IStream.h"

namespace X
{
    /* Class:       ArrayStream
     *
     */
    class ArrayStream : public IStream
    {
    public:
        ArrayStream(byte *, size_t);
        bool WriteBytes(const byte *, int);
        bool ReadBytes(byte *, int);
        bool Skip(int);
    private:
        byte * m_buff;
        size_t m_readPos, m_writePos, m_length;
    };
};

#endif