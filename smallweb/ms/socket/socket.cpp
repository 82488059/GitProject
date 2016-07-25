#include <ms/socket/socket.h>

#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")  //winsock2

namespace ms
{


socket::socket()
    : socket_(INVALID_SOCKET)
    , ip_()
    , port_()
{

}

socket::~socket()
{

}
bool socket::InitSocket()
{
    if (INVALID_SOCKET != socket_)
    {
        return false;
    }

    socket_ = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    return INVALID_SOCKET != socket_;
}
bool socket::CloseSocket()
{
    if (INVALID_SOCKET == socket_)
    {
        return true;
    }
    
    int rVal = closesocket(socket_);
    socket_ = INVALID_SOCKET;
    return true;
}
bool socket::Connect()
{
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port_);
    if (ip_.empty())
    {
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    else
    {
        InetPton(AF_INET, ip_.c_str(), &addr.sin_addr);
    }

    int ret = ::connect(socket_, (sockaddr*)&addr, sizeof(sockaddr_in));
    if (SOCKET_ERROR == ret)
    {
        return false;
    }
    return true;
}
SOCKET socket::Accetp()
{
    sockaddr_in clientAddr = { 0 };
    int len = sizeof(sockaddr_in);
    return accept(socket_, (sockaddr*)&clientAddr, &len);
}


}