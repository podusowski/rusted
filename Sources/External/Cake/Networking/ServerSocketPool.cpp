#include <stdexcept>

#ifdef _WIN32
    #include <windows.h>
#endif

#include "Utils/BuildString.hpp"
#include "Detail/Error.hpp"
#include "ServerSocketPool.hpp"

using namespace Cake::Networking;

void ServerSocketPool::add(std::shared_ptr<ServerSocket> server)
{
    if (server)
    {
        m_servers.push_back(server);
    }
    else
    {
        throw std::logic_error("passed empty server pointer");
    }
}

ServerSocketPool::AcceptReturnType ServerSocketPool::accept()
{
    fd_set sockets;

    FD_ZERO(&sockets);

    int highestNative = 0;

    for (const auto server: m_servers)
    {
        auto native = server->getNativeHandle();
        FD_SET(native, &sockets);

        if (native > highestNative)
        {
            highestNative = native;
        }
    }

    int num = select(highestNative + 1, &sockets, nullptr, nullptr, nullptr);

    if (num < 0)
    {
        throw std::runtime_error(BUILD_STRING << "can't accept new connection, reason: " << Detail::Error::lastError());
    }
    else if (num > 0)
    {
        // got some new socket!
        for (auto server: m_servers)
        {
            auto native = server->getNativeHandle();
            if (FD_ISSET(native, &sockets))
            {
                return std::make_pair(server->accept(), server);
            }
        }
    }

    throw std::runtime_error("huh?");
}
