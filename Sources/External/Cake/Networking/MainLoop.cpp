#include "MainLoop.hpp"

#include "Diagnostics/Logger.hpp"
#include "Utils/BuildString.hpp"
#include "Detail/Error.hpp"

#ifdef _WIN32
    #include <windows.h>
#endif

#include <stdexcept>

namespace Cake
{
namespace Networking
{

MainLoop::MainLoop(Servers servers) : m_servers(servers)
{
    LOG_INFO << "Entering main loop with with " << m_servers.size() << " server(s)";
}

void MainLoop::run()
{
    try
    {
        while (true)
        {
            wait();
        }
    }
    catch (std::exception & ex)
    {
        LOG_WARN << "Main loop has ended, reason: " << ex.what();
    }
}

void MainLoop::wait()
{
    fd_set sockets;

    FD_ZERO(&sockets);

    int highestNative = 0;

    for (const auto server: m_servers)
    {
        auto native = server->getNativeHandle();
        FD_SET(native, &sockets);

        highestNative = std::max(native, highestNative);
    }

    int num = select(highestNative + 1, &sockets, nullptr, nullptr, nullptr);

    if (num < 0)
    {
        throw std::runtime_error(BUILD_STRING << "error from select, reason: " << Detail::Error::lastError());
    }
    else if (num > 0)
    {
        for (auto server: m_servers)
        {
            auto native = server->getNativeHandle();
            if (FD_ISSET(native, &sockets))
            {
                server->act();
            }
        }
    }
    else
    {
        throw std::runtime_error("huh?");
    }
}

}
}
