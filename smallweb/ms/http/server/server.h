#pragma once
#include <ms/socket/socket.h>


namespace ms{
namespace http{

    class server
    {
    public:
        server();
        ~server();

        bool Start();
    private:
        ms::tcp::socket socket_;
    };



}
}
