#include "IStream.h"
#include "common.h"

namespace X
{
    /*
     *
     */
    bool IStream::SetByteOrder(IStream::ByteOrder order)
    {
        m_byteOrder = order;
        return true;
    }

    bool IStream::ReadByte(byte & b)
    {
        byte b1;
        if(!ReadBytes(&b1, sizeof(byte)))
            return false;
        b = b1;
        return true;
    }

    /*
     *
     */
    bool IStream::WriteByte(byte b)
    {
        if(!WriteBytes(&b, sizeof(byte)))
            return false;
        return true;
    }

    /* Function:        IStream::ReadUint16
     * Description:     Reads a 16 bit unsigned integer
     */
    bool IStream::ReadUint16(uint16 & num)
    {
        uint16 n;
        if(!ReadBytes((byte *) &n, sizeof(uint16)))
            return false;
        
#ifdef __LITTLE_ENDIAN__
        if(m_byteOrder == BigEndian) {
            num = SWAPBYTES_16(n);
        } else {
            num = n;
        }
#else
        if(m_byteOrder == LittleEndian) {
            num = SWAPBYTES_16(n);
        } else {
            num = n;
        }
#endif
        return true;
    }

    /*
     *
     */
    /* Function:        IStream::ReadUint16
     * Description:     Reads a 16 bit unsigned integer
     */
    bool IStream::ReadInt16(int16 & num)
    {
        int16 n;
        if(!ReadBytes((byte *) &n, sizeof(int16)))
            return false;
        
#ifdef __LITTLE_ENDIAN__
        if(m_byteOrder == BigEndian) {
            num = SWAPBYTES_16(n);
        } else {
            num = n;
        }
#else
        if(m_byteOrder == LittleEndian) {
            num = SWAPBYTES_16(n);
        } else {
            num = n;
        }
#endif
        return true;
    }

    /*
     *
     */
    bool IStream::ReadUint32(uint32 & num)
    {
        uint32 n;
        if(!ReadBytes((byte *) &n, sizeof(uint32)))
            return false;
        
#ifdef __LITTLE_ENDIAN__
        if(m_byteOrder == BigEndian) {
            num = SWAPBYTES_32(n);
        } else {
            num = n;
        }
#else
        if(m_byteOrder == LittleEndian) {
            num = SWAPBYTES_32(n);
        } else {
            num = n;
        }
#endif
        return true;
    }

    /*
     *
     */
    bool IStream::ReadInt32(int32 & num)
    {
        int32 n;
        if(!ReadBytes((byte *)&n, sizeof(uint32)))
            return false;
        
#ifdef __LITTLE_ENDIAN__
        if(m_byteOrder == BigEndian) {
            num = SWAPBYTES_32(n);
        } else {
            num = n;
        }
#else
        if(m_byteOrder == LittleEndian) {
            num = SWAPBYTES_32(n);
        } else {
            num = n;
        }
#endif
        return true;
    }

    /* 
     *
     */


    bool IStream::WriteUint16(uint16 num)
    {
#ifdef __LITTLE_ENDIAN__
        if(m_byteOrder == BigEndian) {
            num = SWAPBYTES_16(num);
        }
#else
        if(m_byteOrder == LittleEndian) {
            num = SWAPBYTES_16(num);
        }
#endif
        if(!WriteBytes((byte *)&num, sizeof(uint16)))
            return false;
        return true;
    }

    bool IStream::WriteInt16(int16 num)
    {
#ifdef __LITTLE_ENDIAN__
        if(m_byteOrder == BigEndian) {
            num = SWAPBYTES_16(num);
        }
#else
        if(m_byteOrder == LittleEndian) {
            num = SWAPBYTES_16(num);
        }
#endif
        if(!WriteBytes((byte *)&num, sizeof(int16)))
            return false;
        return true;
    }

    bool IStream::WriteUint32(uint32 num)
    {
#ifdef __LITTLE_ENDIAN__
        if(m_byteOrder == BigEndian) {
            num = SWAPBYTES_32(num);
        }
#else
        if(m_byteOrder == LittleEndian) {
            num = SWAPBYTES_32(num);
        }
#endif
        if(!WriteBytes((byte *) &num, sizeof(uint32)))
            return false;
        return true;
    }

    bool IStream::WriteInt32(int32 num)
    {
#ifdef __LITTLE_ENDIAN__
        if(m_byteOrder == BigEndian) {
            num = SWAPBYTES_32(num);
        }
#else
        if(m_byteOrder == LittleEndian) {
            num = SWAPBYTES_32(num);
        }
#endif
        if(!WriteBytes((byte *) &num, sizeof(uint32)))
            return false;
        return true;
    }
};