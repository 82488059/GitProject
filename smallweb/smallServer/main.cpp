#include <iostream>

#include <ms/socket/socket.h>

#include <ms/http/server/server.h>

int main()
{
//     while (1)
//     {
//         int a = 0;
//         int b = 1000;
//         int c = a + b;
//         int n = 0;
//         std::cin >> n;
//         int sum = 0;
//         for (int i = 0; i < n; ++i)
//         {
//             c = a + b;
//             a = b;
//             b = c;
//             std::cout << c << std::endl;
//         }
//     }

    WORD wVer = MAKEWORD(1, 1);
    WSADATA wsaData;
    int nRet = ::WSAStartup(wVer, &wsaData);
    if (0 != nRet)
    {
        return -1;
    }
    ms::http::server s1;

    s1.Start();

    WSACleanup();
    system("pause");
    return 0;
}