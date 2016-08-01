#pragma once

#include <winsock2.h>
#include <ms/mstring.h>

namespace ms{
namespace tcp{

    class socket
    {
    public:
        socket();
        socket(SOCKET s)
            :socket_(s)
        {
        }
        socket(const socket& s)
            :socket_(s.socket_)
            , ip_(s.ip_)
            , port_(s.port_)
        {
        }
        ~socket();

        bool InitSocket();
        bool CloseSocket();
        bool Listen();
        bool Connect();
        int Recv(char* buf, int bufsize, int flags);
            
        void SetIpPort(const ms::string& ip, short port);
        SOCKET Accetp();
    public:
        SOCKET socket_;
        ms::string ip_;
        short port_;
        //addr
    };
}
}