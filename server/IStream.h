#ifndef _ISTREAM_H_
#define _ISTREAM_H_

#include "types.h"

namespace X
{
    /* Class:           IStream
     * Description:     I/O stream class.
     */
    class IStream
    {
    public:
        enum ByteOrder {LittleEndian, BigEndian};
        virtual bool WriteBytes(const byte *, int) = 0;
        virtual bool ReadBytes(byte *, int) = 0;
        bool WriteByte(byte);
        bool WriteUint32(uint32);
        bool WriteUint16(uint16);
        bool WriteInt32(int32);
        bool WriteInt16(int16);
        
        bool ReadByte(byte &);
        bool ReadUint32(uint32 &);
        bool ReadUint16(uint16 &);
        bool ReadInt32(int32 &);
        bool ReadInt16(int16 &);
        //bool Skip(int);
        

        bool SetByteOrder(ByteOrder bo = LittleEndian);
    protected:
        ByteOrder m_byteOrder;
    };
};

#endif