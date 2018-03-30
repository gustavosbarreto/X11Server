#include "XClient.h"
#include "XServer.h"
#include <iostream>
#include "common.h"
#include <assert.h>
#include "ArrayStream.h"
#include "XDefines.h"

using namespace std;

namespace X
{
    /* Function:        XClient::XClient
     * Description:     Constructor.
     */
    XClient::XClient(XServer * server, SocketLayer * sock)
    {
        m_server    = server;               // set the server.
        m_socket    = sock;
        // null the pointers
        m_readInfo.buffer = NULL;
        m_sendInfo.buffer = NULL;
    }

    /* Function:        XClient::~XClient
     * Description:     Destructor, performs the required cleanup.
     */
    XClient::~XClient()
    {   
        delete [] m_readInfo.buffer;
        delete [] m_sendInfo.buffer;
    }

    /* Function:        XClient::isMBMMode
     * Description:     
     */
    bool XClient::isMSBMode()
    {
        return (m_clientHdr.byteOrder == 'B');
    }

    /* Function:        XClient::ReadClientInfo
     * Description:     The read process is divided into to parts. First we read the "header" which is the 
     *                  fixed length part of the message. Based on this header we calculate the remaining size and
     *                  reads it as a single part.
     */
    int XClient::ReadClientInfo()
    {
        int nret;
        // First read the header.
        if(m_readInfo.bytesRead < sizeof(m_clientHdr)) {
            // havent read the header yet
            nret = m_socket->Read(((byte *)&m_clientHdr) + m_readInfo.bytesRead, sizeof(m_clientHdr) - m_readInfo.bytesRead);
            if(nret == 0) {
                // the connection was closed
                cerr << "ReadClientInfo: Connection closed by the remote host" << endl;
                return X_FATAL_ERROR;
            } else if(nret < 0) {
                // a error occurred.
                cerr << "ReadClientInfo: Error while reading the client information" << endl;
                return X_FATAL_ERROR;
            } else {
                m_readInfo.bytesRead += nret;
                if(m_readInfo.bytesRead == sizeof(m_clientHdr))
                {
                    // the header has been read, now swap the bytes if required.
                    if(m_clientHdr.byteOrder == 0x42) {
#if defined(_DEBUG_VERBOSE)
                        cerr << "ReadClientInfo: byte order set to MSB" << endl;
#endif
                        m_clientHdr.majorVersion    = SWAPBYTES_16(m_clientHdr.majorVersion);
                        m_clientHdr.minorVersion    = SWAPBYTES_16(m_clientHdr.minorVersion);
                        m_clientHdr.d               = SWAPBYTES_16(m_clientHdr.d);
                        m_clientHdr.n               = SWAPBYTES_16(m_clientHdr.n);
                    } else if(m_clientHdr.byteOrder != 0x6C) {
                        cerr << "ReadClientInfo: Invalid value on the byte order specifier" << endl;
                        return -1;
                    } else {
#if defined(_DEBUG_VERBOSE)
                        cerr << "ReadClientInfo: byte order set to LSB" << endl;
#endif
                    }
                    // allocate a buffer to store the authentication data in.
                    m_authsize = m_clientHdr.n + pad(m_clientHdr.n,8) + m_clientHdr.d + pad(m_clientHdr.d,8);
                    m_authbuffer = new (std::nothrow) unsigned char[m_authsize];
                    if(m_authbuffer == NULL) {
                        cerr << "Could not allocate the buffer to store the client authentication data in" << endl;
                        return -1;
                    }
                }
            }
        } else if(m_readInfo.bytesRead < (sizeof(m_clientHdr) + m_authsize)) {
            // haven't read tha authentication data yet.
            assert(m_readInfo.bytesRead >= sizeof(m_clientHdr));
            size_t offset = m_readInfo.bytesRead - sizeof(m_clientHdr);
            nret = m_socket->Read(m_authbuffer + offset, m_authsize - offset);
            if(nret == 0) {
                // the connection was closed
                cerr << "ReadClientInfo: The connection was closed by the remote side" << endl;
                return -1;
            } else if(nret < 0) {
                // a error occurred.
                cerr << "ReadClientInfo: A fatal error occurred while reading the data from teh server" << endl;
                return -1;
            } else {
                m_readInfo.bytesRead += nret;
                if(m_readInfo.bytesRead == (sizeof(m_clientHdr) + m_authsize))
                {
                    // have read all of the clients authentication data
                    return X_STATUS_SUCCESS;
                } 
                return X_STATUS_PENDING;
            }
        } 
        // should not happen
        assert(false);
        return FATAL_ERROR;
    }

    /* Function:        XClient::WriteServerInfo
     * Description:     Writes the server info to the send buffer.
     */
    int XClient::WriteServerInfo(const ServerInfo & info, IStream * os)
    {
        static byte padding[] = {0,0,0,0,0,0,0,0};
        // calculate the padding size.
        uint32 vendorPadding = (info.vendorLength % 8 ? (8 - info.vendorLength) : 0);
        // write the server info.
        if(!os->WriteUint16(info.majorVersion) || 
            !os->WriteUint16(info.minorVersion) ||
            !os->WriteUint16(8 + 2 * static_cast<uint16>(info.pixmapFormats.size()) + (info.vendorLength + vendorPadding) / 4) ||
            !os->WriteBytes(info.vendor, info.vendorLength) ||
            !os->WriteUint32(info.releaseNumber) ||
            !os->WriteUint32(info.resourceIdBase) ||
            !os->WriteUint32(info.resourceIdMask) ||
            !os->WriteUint32(info.motionBufferSize) ||
            !os->WriteUint16(info.vendorLength) ||
            !os->WriteUint16(info.maxRequestLength) ||
            !os->WriteByte(static_cast<byte>(info.roots.size())) ||
            !os->WriteByte(static_cast<byte>(info.pixmapFormats.size())) ||
            !os->WriteByte(info.imageByteOrder) ||
            !os->WriteByte(info.bitmapScanlineUnit) ||
            !os->WriteByte(info.bitmapScanlinePad) ||
            !os->WriteByte(info.minKeycode) ||
            !os->WriteByte(info.maxKeycode) ||
            !os->WriteUint32(0) ||
            !os->WriteBytes(info.vendor, info.vendorLength) ||
            !os->WriteBytes(padding, vendorPadding) ||
            !writeFormatList(info.pixmapFormats, os) ||
            !writeScreenList(info.roots, os))
        {
            cerr << "Could not write the Server info" << endl;
            return FATAL_ERROR;
        }
        return STATUS_SUCCESS;
    }


    /* Function:        XClient::ReadRequest
     * Description:     Reads a request in a non-blocking fashion.
     */
    int XClient::ReadRequest()
    {
        int nret;
        if(m_readInfo.bytesRead < sizeof(RequestHdr))
        {
            // haven't read the request header yet.
            nret = m_socket->Read(&m_readBuffer[m_readInfo.bytesRead], sizeof(RequestHdr) - m_readInfo.bytesRead);
            if(nret < 0) {
                // a error occurred.
                cerr << "ReadRequest: Failed to read the request" << endl;
                return -1;
            } else if(nret == 0) {
                // connection closed.
                cerr << "ReadRequest: The connection was closed by the remote host" << endl;
                return -1;
            } 
            m_bytesRead += nret;
            if(m_readInfo.bytesRead == sizeof(RequestHdr))
            {
                // We have read the request header.
                if(isMSBMode())
                    m_readHdr->size = SWAPBYTES_16(m_readHdr->size);
#if defined(_DEBUG_VERBOSE)
                cerr << "ReadRequest: Got a request of type: " << (int) m_readHdr->opcode << " with the size " 
                    << (m_readHdr->size << 2) << " bytes (" << m_readHdr->size << ")." << endl;
#endif
                // calculate the size to read.
                size_t requestSize = (m_readHdr->size * 4);
                m_readInfo.transferSize = requestSize;
                if(requestSize < sizeof(m_readHdr)) 
                {
                    cerr << "ReadRequest: Invalid size field included in the request header" << endl;
                    return X_FATAL_ERROR;
                }
                else if(requestSize == sizeof(m_readHdr)) 
                {
                    // the request has been read.
                    return REQUEST_COMPLETE;
                }
                if(m_readInfo.buffer == NULL)
                {
                    m_readInfo.buffer = new (std::nothrow) unsigned char[requestSize];
                    if(m_readInfo.buffer == NULL) {
                        cerr << "ReadRequest: Failed to allocate the readbuffer" << endl;
                        return -1;
                    }
                    m_readInfo.bufferSize = requestSize;
                } else if(requestSize > m_readInfo.bufferSize)
                {
                    // we need to increase the buffer size.
#if defined(_DEBUG_VEROSE)
                    cerr << "ReadRequest (debug): Need to increase the read buffer from " << readInfo.bufferSize 
                        << " bytes, to " << requestSize <<" bytes" << endl;
#endif
                    delete [] m_readInfo.buffer; m_readInfo.buffer = NULL;
                    m_readInfo.buffer = new (std::nothrow) unsigned char[requestSize];
                    if(m_readInfo.buffer == NULL) {
                        cerr << "ReadRequest: Failed to increase the read buffer, memory allocation failed" << endl;
                        return -1;
                    }
                    m_readInfo.bufferSize = requestSize;
                }
            }
        } 
        if(m_readInfo.bytesRead >= sizeof(RequestHdr))
        {
            // have read the header, but not the entire body.
            assert(m_readInfo.buffer != NULL);
            assert(m_readInfo.bytesRead < m_readInfo.transferSize);

            if(m_readInfo.buffer == NULL) {
                cerr << "ReadRequest: The read buffer isn't allocated" << endl;
                return -1;
            }
            size_t offset = m_readInfo.bytesRead - sizeof(RequestHdr);
            nret = m_socket->Read(m_readInfo.buffer + offset, offset - m_readInfo.transferSize);
            if(nret < 0) {
                cerr << "ReadRequest: Failed to read the request" << endl;
                return -1;
            } else if(nret == 0) {
                cerr << "ReadRequest: The connection was closed by the remote host" << endl;
                return -1;
            } else {
                m_readInfo.bytesRead += nret;
                if(m_readInfo.bytesRead == m_readInfo.transferSize) {
                    // have read the entire request.
                    return REQUEST_COMPLETE;
                } else 
                    return REQUEST_PENDING;
            }
        } else {
            // should never happen
            assert(false);
            return FATAL_ERROR;
        }
    }

    /* Function:        XClient::Process    
     * Description:     Handles the client communication between the client and the server.
     */
    int XClient::Process()
    {
        return 0;
    }

    /* Function:        XClient::writeFormatList
     * Description:     Writes a format list.
     */
    bool XClient::writeFormatList(const std::list<FormatType> & formats, IStream * os)
    {
        static byte padding[] = {0,0,0,0,0};
        for(std::list<FormatType>::const_iterator it = formats.begin();
            it != formats.end();
            it++)
        {
            if(!os->WriteByte(it->depth) ||             // write the depth
                !os->WriteByte(it->bitsPerPixel) ||     // write the number of bits per pixel
                !os->WriteByte(it->scanlinePadd) ||     // write the scanline padding
                !os->WriteBytes(padding, 5))            // write the padding
            {
                cerr << "Could not write the format list" << endl;
                return false;
            }
        }
        return true;
    }

    /*
     *
     */
    bool XClient::writeScreenList(const std::list<ScreenType> & screens, IStream * os)
    {
        for(std::list<ScreenType>::const_iterator it = screens.begin();
            it != screens.end();
            it++)
        {
            if(!os->WriteUint32(it->root) ||
                !os->WriteUint32(it->defaultColormap) ||
                !os->WriteUint32(it->whitePixel) ||
                !os->WriteUint32(it->blackPixel) ||
                !// write events here        ||
                !os->WriteUint16(it->width_pixels) ||
                !os->WriteUint16(it->height_pixels) ||
                !os->WriteUint16(it->width_mms) ||
                !os->WriteUint16(it->height_mms) ||
                !os->WriteUint16(it->minInstalledMaps) ||
                !os->WriteUint16(it->maxInstalledMaps) ||
                !os->WriteUint32(it->rootVisual) ||
                !os->WriteByte(it->backingStore) ||
                !os->WriteByte(it->saveUnders) ||
                !os->WriteByte(it->rootDepth) ||
                !os->WriteByte(static_cast<byte>(it->listOfDepths.size())) ||
                !writeDepthList(it->listOfDepths, os))
            {
                cerr << "Could not write the screen list" << endl;
                return false;
            }
        }
        return true;
    }

    /*
     *
     */
    bool XClient::writeDepthList(const std::list<DepthType> & depths, IStream * os)
    {
        for(std::list<DepthType>::const_iterator it = depths.begin();
            it != depths.end();
            it++)
        {
            if(!os->WriteByte(it->depth) ||
                !os->WriteByte(0) ||
                !os->WriteUint16(static_cast<uint16>(it->visuals.size())) ||
                !os->WriteUint32(0) ||
                !writeVisualList(it->visuals, os))
            {
                cerr << "Could not write the depth list" << endl;
                return false;
            }
        }
        return true;
    }

    /*
     *
     */
    bool XClient::writeVisualList(const std::list<VisualType> & visuals, IStream * os)
    {
        for(std::list<VisualType>::const_iterator it = visuals.begin();
            it != visuals.end();
            it++)
        {
            if(!os->WriteUint32(it->id) ||
                !os->WriteByte(it->type_class) ||
                !os->WriteByte(it->bits_per_rgb_value) ||
                !os->WriteUint16(it->colourmap_entries) ||
                !os->WriteUint32(it->red_mask) ||
                !os->WriteUint32(it->green_mask) ||
                !os->WriteUint32(it->blue_mask) ||
                !os->WriteUint32(0))
            {
                cerr << "Could not write the Visual list" << endl;
                return false;
            }
        }
        return true;
    }

    /* Function:        XClient::SupportedVersion
     * Description:     Returns true if the current version is supported.
     */
    bool XClient::SupportedVersion(int major, int minor)
    {
        if(major == 11)
            return true;
        else
            return false;
    }

    /* Function:        XClient::SendPacket_nonblock
     * Description:     Sends a packet to the client.
     */
    int XClient::SendPacket_nonblock()
    {
        return -1;
    }

    
    /* Function:        XClient::AddEvent
     * Description:     Adds a event to the event queue.
     */
    void XClient::AddEvent(const XEvent & ev)
    {
        m_events.push_back(ev);
    }
};