#include <stdexcept>

#ifdef _WIN32
    #include <winsock2.h>
    #include <windows.h>
#else
    #include <sys/socket.h>
    #include <sys/un.h>
    #include <netinet/in.h>
    #include <netdb.h>
#endif

#include "SocketInitialize.hpp"

using namespace Cake::Networking;

void SocketInitialize::tryInitialize()
{
#ifdef _WIN32
    if (!s_initialized)
    {
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2,2), &wsaData);
        if (result != 0)
        {
            throw std::runtime_error("WSAStartup failed");
        }
        s_initialized = true;
    }
#endif
}

bool SocketInitialize::s_initialized = false;

