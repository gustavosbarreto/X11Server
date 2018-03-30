#include "ArrayStream.h"

namespace X
{
    ArrayStream::ArrayStream(byte * ptr, size_t length)
    {
        m_buff = ptr;
        m_length = length;
        m_readPos = m_writePos = 0;
    }

    /*
     *
     */
    bool ArrayStream::ReadBytes(byte * dst, int len)
    {
        if(len <= 0) return false;
        if(m_readPos + len > m_length)
            return false;
        memcpy(dst, &m_buff[m_readPos], len);
        m_readPos += len;
        return true;
    }

    /*
     *
     */
    bool ArrayStream::WriteBytes(const byte * src, int len)
    {
        if(len <= 0)
            return false;
        if((m_writePos + len) > m_length)
            return false;
        memcpy(&m_buff[m_writePos], src, len);
        m_writePos += len;
        return true;
    }

    /*
     *
     */
    bool ArrayStream::Skip(int num)
    {
        if(num < 0) return false;
        if((m_readPos + num) > m_length)
            return false;
        m_readPos += num;
        return true;
    }
};