#include "SocketLayer.h"
#include <memory>
#include <iostream>
#include "common.h"

namespace X
{
    using namespace std;

    /* Function:        SocketLayer::SocketLayer
     * Description:     Constructor.
     */
    SocketLayer::SocketLayer(XSOCKET sock)
    {
        m_socket = sock;
    }

    /* Function:        SocketLayer::SocketLayer
     * Description:     Constructor.
     */
    SocketLayer::SocketLayer()
    {
        m_socket = 0;
    }

    /* Function:        SocketLayer::~SocketLayer
     * Description:     Performs the required cleanup
     */
    SocketLayer::~SocketLayer()
    {
        if(m_socket) closesocket(m_socket);
    }

    /* Function:        SocketLayer::Bind
     * Description:     Binds the socket to a interface.
     */
    int SocketLayer::Bind(const char * addr, unsigned short port)
    {
        m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(m_socket == INVALID_SOCKET) {
            cerr << "Coukd not create the socket" << endl;
            return FATAL_ERROR;
        }
        sockaddr_in service;
        service.sin_family = AF_INET;
        service.sin_addr.s_addr = inet_addr(addr);
        service.sin_port = htons(port);
    
        if(bind(m_socket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
        {
            cerr << "Could not bind to the address " << addr << " on port " << port << endl;
            return FATAL_ERROR;
        }
        return STATUS_SUCCESS;
    }

    /* Function:        SocketLayer::Listen
     * Description:     Places the socket in a state listening for new connections.
     */
    int SocketLayer::Listen(int backlog)
    {
        if(listen(m_socket, backlog) == SOCKET_ERROR) {
            cerr << "listen() failed" << endl;
            return FATAL_ERROR;
        }
        return STATUS_SUCCESS;
    }

    /* Function:        SocketLayer::isClientAvailable
     * Description:     Checks if any new client is available.
     */
    int SocketLayer::isClientAvailable()
    {
        static const timeval t = {0,0};
        fd_set set;
        FD_ZERO(&set);
        FD_SET(m_socket, &set);
        int nret = select(0, NULL, &set, 0, &t);
        if(nret == SOCKET_ERROR) 
            return FATAL_ERROR;
        return (nret == 0 ? 0 : 1);;
    }

    /* Function:        SocketLayer::isDataAvailable
     * Description:     Returns true if data is available to read.
     */
    int SocketLayer::isDataAvailable()
    {
        static const timeval t = {0,0};
        fd_set set;
        FD_ZERO(&set);
        FD_SET(m_socket, &set);
        int nret = select(0, &set, 0, 0, &t);
        if(nret == SOCKET_ERROR) 
            return FATAL_ERROR;
        return (nret == 0 ? 0 : 1);
    }

    /* Function:        SocketLayer::AcceptClient
     * Description:     Accepts a client.
     */
    SocketLayer * SocketLayer::AcceptClient()
    {
        sockaddr_in service;
        int size;
        XSOCKET sock = accept(m_socket, (SOCKADDR*)&service, &size);
        if(sock == INVALID_SOCKET) {
            cerr << "accept() failed" << endl;
            return NULL;
        }
        SocketLayer * client = new (std::nothrow) SocketLayer(sock);
        if(client == NULL) {
            cerr << "Could not create the SocketLayer instance for the client" << endl;
            closesocket(sock);
            return NULL;
        }
        return client;
    }

    /*
     *
     */
    void SocketLayer::close()
    {
        closesocket(m_socket);
    }

    /*
     *
     */
    int SocketLayer::Read(byte * dst, int len)
    {
        if(len == 0 || len < 0) {
            return FATAL_ERROR;
        }
        int nret = recv(m_socket, (char *) dst, len, 0);
        if(nret == 0) {
            return CLIENT_DISCONNECTED;
        } else if(nret < 0) {
            return FATAL_ERROR;
        } else {
            // return the amount actual read.
            return nret;
        }
    }

};