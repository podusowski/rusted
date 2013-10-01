#include <stdexcept>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <sys/un.h>
#endif

#include <errno.h>
#include <sstream>
#include <string.h>
#include <cstdio>

#include "ServerSocket.hpp"

using namespace Cake::Networking;

ServerSocket::ServerSocket(int sockFd) :
    m_sockFd(sockFd)
{
}

// TODO: refactor
std::shared_ptr<ServerSocket> ServerSocket::createTcpServer(unsigned port)
{
    int sockFd = ::socket(PF_INET, SOCK_STREAM, 0);

    if (sockFd == -1)
    {
        throw std::runtime_error("can't create socket");
    }

    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (::bind(sockFd, (sockaddr*)&addr, sizeof(addr)) == -1)
    {
        throw std::runtime_error("can't bind socket");
    }

    if (::listen(sockFd, 5) == -1)
    {
        throw std::runtime_error("can't listen to the socket");
    }

    //TODO: make_shared
    return std::shared_ptr<ServerSocket>(new ServerSocket(sockFd));
}

std::shared_ptr<ServerSocket> ServerSocket::createUnixServer(const std::string & path)
{
#ifdef _WIN32
    throw std::runtime_error("WIN32 doesn't support UNIX sockets");
#else
    int sockFd = ::socket(PF_UNIX, SOCK_STREAM, 0);

    if (sockFd == -1)
    {
        throw std::runtime_error("can't create socket");
    }

    sockaddr_un local_addr;
    local_addr.sun_family = PF_UNIX;
    ::strcpy(local_addr.sun_path, path.c_str());
    ::remove(local_addr.sun_path);

    ::bind(sockFd, 
           (sockaddr *)(&local_addr), 
           sizeof(local_addr.sun_family) + strlen(local_addr.sun_path));

    if (::listen(sockFd, 5) == -1)
    {
        throw std::runtime_error("can't listen to the socket");
    }

    return std::shared_ptr<ServerSocket>(new ServerSocket(sockFd));
#endif
}

std::shared_ptr<Socket> ServerSocket::accept()
{
    int sockFd = ::accept(m_sockFd, NULL, NULL);

    if (sockFd == -1)
    {
        std::stringstream ss;
        ss << "can't accept new connection, reason: " << strerror(errno); 
        throw std::runtime_error(ss.str());
    }

    return std::shared_ptr<Socket>(new Socket(sockFd));
}

int ServerSocket::getNativeHandle() const
{
    return m_sockFd;
}
