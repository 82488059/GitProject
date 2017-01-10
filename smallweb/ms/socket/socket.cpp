#include <ms/socket/socket.h>

#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")  //winsock2

namespace ms
{
namespace tcp{

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
    bool socket::Listen()
    {
        int nRet = -1;

        SOCKADDR_IN addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port_);
        if (ip_.empty())
        {
            addr.sin_addr.s_addr = htonl(INADDR_ANY);
            int iOptVal = 1;
            int iOptLen = sizeof(int);
            nRet = setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, (char*)&iOptVal, iOptLen);
        }
        else
        {
            InetPton(AF_INET, ip_.c_str(), &addr.sin_addr);
        }
        nRet = ::bind(socket_, (LPSOCKADDR)&addr, sizeof(sockaddr));
        if (SOCKET_ERROR == nRet)
        {
            closesocket(socket_);
            socket_ = INVALID_SOCKET;
            return false;
        }
        nRet = listen(socket_, 5); //listen connection 
        if (SOCKET_ERROR == nRet)
        {
            closesocket(socket_);
            socket_ = INVALID_SOCKET;
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
    void socket::SetIpPort(const ms::string& ip, short port)
    {
        ip_ = ip;
        port_ = port;
    }
    int socket::Recv(char* buf, int bufsize, int flags)
    {
        return recv(socket_, buf, bufsize, flags);
    }

}
}