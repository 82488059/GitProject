#include <ms/http/server/server.h>


#include <ms/http/server/session.h>

#include <memory>

namespace ms{
namespace http{

    server::server()
    {
    }

    server::~server()
    {
    }
    bool server::Start()
    {
        socket_.SetIpPort(_T(""), 8888);
        if (!socket_.InitSocket())
        {
            return false;
        }
        if (!socket_.Listen())
        {
            return false;
        }

        while (1)
        {
            std::shared_ptr<ms::http::session> ss(new ms::http::session, [](ms::http::session* p){
                p->Wait();
            });
            (*ss).socket_ = socket_.Accetp();
            if ((*ss).socket_.socket_ == INVALID_SOCKET)
            {
                break;
            }
            (*ss).Start();
        }


        return true;
    }



}
}