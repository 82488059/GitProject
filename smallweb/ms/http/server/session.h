#pragma once
#include <ms/socket/socket.h>
#include <thread>
#include <memory>

namespace ms{
namespace http{

    class session
    {
    public:
        session();
        session(const ms::tcp::socket& s);
        ~session();

        bool Start();

        void ThreadProc();
        void Wait();

    public:
        ms::tcp::socket socket_;
        std::shared_ptr<std::thread> threadPtr_;
    };



}
}
