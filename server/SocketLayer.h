#ifndef _SOCKETLAYER_H_
#define _SOCKETLAYER_H_

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#define SOCKET XSOCKET
#include <windows.h>
#include <winsock2.h>
#endif

#include "types.h"

namespace X
{
    /* Class:           SocketLayer
     * Description:     Handles the socket communication. All communication is
     *                  non-blocking
     */
    class SocketLayer
    {
    public:
        SocketLayer(XSOCKET);
        SocketLayer();
        ~SocketLayer();

        int Read(byte *, int len);          // reads data from the socket
        int Write(const byte *, int len);   // writes data to the socket

        // binds the socket to the 
        int Bind(const char * addr, unsigned short port);
        int Listen(int backlog);
        int Connect(const char * addr, const char * port);

        int isDataAvailable();
        int isClientAvailable();

        void close();
        SocketLayer * AcceptClient();

    protected:
#ifdef _WIN32
        XSOCKET m_socket;
#endif
    };
};

#endif