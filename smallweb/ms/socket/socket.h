#pragma once

#include <winsock2.h>
#include <ms/mstring.h>

namespace ms{

    class socket
    {
    public:
        socket();
        ~socket();
        bool InitSocket();
        bool CloseSocket();
        bool Connect();
        void SetIpPort(const ms::string& ip, short port);
        SOCKET Accetp();
    private:
        SOCKET socket_;
        ms::string ip_;
        short port_;
        //addr
    };

}