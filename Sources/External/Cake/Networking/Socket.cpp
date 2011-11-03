#include <stdexcept>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>

#include "Socket.hpp"
#include "System/AtomicSyscall.hpp"
#include "DnsResolver.hpp"
#include "Detail/SockFdGuard.hpp"

using namespace Cake::Networking;

Socket::Socket(int sockFd) : m_sockFd(sockFd)
{
    m_stream.reset(new SocketStream(sockFd));
}

Socket::~Socket()
{
    ::close(m_sockFd);
}

boost::shared_ptr<Socket> Socket::connectToUnixSocket(const std::string & path)
{
    Detail::SockFdGuard sockFd;
    ATOMIC_SYSCALL(::socket(AF_UNIX, SOCK_STREAM, 0), sockFd, == -1);

    sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, path.c_str());

    int result;
    ATOMIC_SYSCALL(::connect(*sockFd, (sockaddr*)&addr, strlen(addr.sun_path) + sizeof(addr.sun_family)), result, == -1);
    
    return boost::shared_ptr<Socket>(new Socket(sockFd.release()));
}

boost::shared_ptr<Socket> Socket::connectToTcpSocket(const std::string & address, int port)
{
    Detail::SockFdGuard sockFd;
    ATOMIC_SYSCALL(::socket(PF_INET, SOCK_STREAM, 0), sockFd, == -1);

    DnsResolver resolver;
    sockaddr_in addr = resolver.resolve(address, port);

    int result;
    ATOMIC_SYSCALL(::connect(*sockFd, (sockaddr*)&addr, sizeof(sockaddr_in)), result, == -1);
    
    return boost::shared_ptr<Socket>(new Socket(sockFd.release()));
}

SocketStream & Socket::stream()
{
    if (m_stream)
    {
        return *m_stream;
    }
    else
    {
        throw std::runtime_error("no stream was created, socket might be disconnected");
    }
}
