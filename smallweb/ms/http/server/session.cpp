#include <ms/http/server/session.h>


#include <iostream>

namespace ms{
namespace http{


    session::session()
    {
    }

    session::session(const ms::tcp::socket& s)
        : socket_(s)
    {
    }

    session::~session()
    {
    }
    void session::Wait()
    {
        if (threadPtr_)
        {
            threadPtr_->join();
        }
    }

    void session::ThreadProc()
    {
        enum value_t
        {
            buf_max = 2048
        };
        while (true)
        {
            char buf[buf_max];
            int n = socket_.Recv(buf, buf_max, 0);
            if (n < 0)
            {
                break;
            }
            if (n == 0)
            {
                break;
            }
            if (n < buf_max)
            {
                buf[n] = 0;
                std::cout << buf << std::endl;
                break;
            }

        }
        return ;
    }

    bool session::Start()
    {
        if (threadPtr_)
        {
            return false;
        }
        threadPtr_ = std::make_shared<std::thread>(std::bind(&session::ThreadProc, this));
        return true;
        
    }




}
}