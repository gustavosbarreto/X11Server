#ifndef _XCLIENT_H_
#define _XCLIENT_H_

#include "IStream.h"
#include "ServerInfo.h"
#include "types.h"
#include "SocketLayer.h"
#include <list>
#include "XEvent.h"
#include "RequestHdr.h"
#include "types.h"
#include "RequestStructs.h"
#include "ErrorStruct.h"

namespace X
{
    class XServer;
    class XWindow;

    /*
     *
     */
#pragma pack(push, 1)
    /* Struct:          ClientInitHdr
     * Description:     The Initial "header" sent by the client.
     */
    struct ClientInitHdr
    {
        byte    byteOrder, 
                unused1;
        uint16  majorVersion,   // the major version
                minorVersion,   // the minor version
                n,              // length of the authentication protocol name
                d,              // length of the authentication protocol data
                unused2;
    };

    /* Information about a read/write op */
    struct TransferState
    {
        union
        {
            size_t bytesRead, bytesSent;
        };
        byte * buffer;          // the buffer with the data
        size_t bufferSize;      // the size of the buffer.
        size_t transferSize;
    };

#pragma pack(pop)

    /* Class:           XClient
     * Description:     Represents a remote X client.
     */
    class XClient
    {
    public:
        XClient(XServer *, SocketLayer *);          // constructor
        ~XClient();                                 // destructor

        int SendPacket_nonblock();      // sends the current packet
        int ReadPacket_nonblock();      // reads a packet.

        // writes the server info to the buffer.
        int writeServerInfo(const ServerInfo &);
        // do some work.
        int Process();

        // adds a event to the event queue
        void AddEvent(const XEvent & ev);

        std::list<WINDOW> & GetGrabbedWindows() {return m_grabbedWindows;}
    protected:

        bool isMSBMode();
        /*
         * The different state functions.
         */
        int ReadClientInfo();
        int WriteServerInfo(const ServerInfo &, IStream *);
        int HandleRequests();

        /*
         * I/O functions.
         */
        int ReadClientInformation();
        int ReadRequest();
        int HandleRequest();
        bool SupportedVersion(int, int);
        bool writeFormatList(const std::list<FormatType> &, IStream *);
        bool writeScreenList(const std::list<ScreenType> &, IStream *);
        bool writeDepthList(const std::list<DepthType> &, IStream *);
        bool writeVisualList(const std::list<VisualType> &, IStream *);


        void NewPacket();
        int                     m_sendState, m_readState;   // the states
        byte *                  m_readBuffer;               // read buffer
        byte *                  m_sendBuffer;               // send buffer
        std::list<XEvent>       m_events;                   // the event queue
        std::list<XWindow *>    m_saveSet;
        XServer *               m_server;
        SocketLayer *           m_socket;
        
        uint32 m_bytesRead;
        RequestHdr * m_readHdr;
        std::list<WINDOW>       m_grabbedWindows;

        ClientInitHdr           m_clientHdr;
        TransferState           m_readInfo, m_sendInfo; 
        byte *                  m_authname, * m_auth_data;
        byte *                  m_authbuffer;
        size_t                  m_authsize;


    };
};

#endif