#include <stdexcept>
#include <iterator>

#ifdef _WIN32
    #include <winsock2.h>
    #include <windows.h>
#else
    #include <sys/socket.h>
    #include <sys/un.h>
    #include <netinet/in.h>
    #include <netdb.h>
#endif

#include "Socket.hpp"
#include "System/AtomicSyscall.hpp"
#include "DnsResolver.hpp"
#include "Detail/SockFdGuard.hpp"
#include "Diagnostics/Throw.hpp"
#include "Utils/BuildString.hpp"
#include "Detail/Error.hpp"
#include "Diagnostics/Logger.hpp"
#include "SocketInitialize.hpp"

using namespace Cake::Networking;

Socket::Socket(int sockFd) : m_sockFd(sockFd)
{
    SocketInitialize::tryInitialize();
}

Socket::~Socket()
{
#ifdef _WIN32
    closesocket(m_sockFd);
#else
    ::close(m_sockFd);
#endif
}

std::shared_ptr<Socket> Socket::connectToUnixSocket(const std::string & path)
{
#ifdef _WIN32
    throw std::runtime_error("WIN32 doesn't support UNIX sockets");
#else
    Detail::SockFdGuard sockFd;
    ATOMIC_SYSCALL(::socket(AF_UNIX, SOCK_STREAM, 0), sockFd, == -1);

    sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, path.c_str());

    int result;
    ATOMIC_SYSCALL(::connect(*sockFd, (sockaddr*)&addr, strlen(addr.sun_path) + sizeof(addr.sun_family)), result, == -1);

    return std::shared_ptr<Socket>(new Socket(sockFd.release()));
#endif
}

std::shared_ptr<Socket> Socket::connectToTcpSocket(const std::string & address, int port)
{
    Detail::SockFdGuard sockFd;
    ATOMIC_SYSCALL(::socket(PF_INET, SOCK_STREAM, 0), sockFd, == -1);

    DnsResolver resolver;
    sockaddr_in addr = resolver.resolve(address, port);

    int result = ::connect(*sockFd, (sockaddr*)&addr, sizeof(sockaddr_in));

    if (result == -1)
    {
        throw std::runtime_error(BUILD_STRING << "can't connect, reason: " << Detail::Error::lastError());
    }

    return std::shared_ptr<Socket>(new Socket(sockFd.release()));
}

void Socket::send(const Bytes & bytes)
{
#ifdef _WIN32
    int ret = ::send(m_sockFd, bytes.chars(), bytes.size(), 0);
#else
    int ret = ::send(m_sockFd, bytes.chars(), bytes.size(), MSG_NOSIGNAL);
#endif

    if (ret == -1)
    {
        throw std::runtime_error(BUILD_STRING << "can't send data, reason: " << Detail::Error::lastError());
    }
}

auto Socket::receive(size_t size) -> Bytes
{
    Bytes bytes(size);
    ptrdiff_t position = 0;

    while (size > 0)
    {
        int ret = ::recv(m_sockFd, std::next(bytes.chars(), position), size, 0);

        if (ret == 0)
        {
            throw std::runtime_error("connection closed by remote during recv");
        }
        else if (ret == -1)
        {
            THROW(std::runtime_error) << "recv returned error: " << Detail::Error::lastError();
        }

        position += ret;
        size -= ret;
    }

    return bytes;
}

void Socket::asyncReceive(size_t expecting, DataReceived dataReceived)
{
    if (m_expecting > 0)
    {
        throw std::runtime_error("socket is already awating for data");
    }

    m_expecting = expecting;
    m_buffer.resize(m_expecting);
    m_dataReceived = dataReceived;
}

Socket & Socket::operator>>(unsigned & value)
{
    auto bytes = receive(sizeof(value));
    value = bytes.as<unsigned>();
    return *this;
}

Socket & Socket::operator<<(unsigned value)
{
    send(Bytes::from(value));
    return *this;
}

Socket & Socket::operator>>(std::string & str)
{
    // TODO: we can operate on the string directly
    str.clear();

    size_t size = receive(sizeof(size_t)).as<size_t>();

    if (size > 0)
    {
        if (size > 1024 * 1024) // 1MB
        {
            throw std::runtime_error("size of string exceeded 1MB, can't receive");
        }

        str = receive(size).chars();
    }

    return *this;
}

Socket & Socket::operator<<(const std::string & str)
{
    size_t size = str.length();
    send(Bytes::from(size));

    if (size > 0)
    {
        send(Bytes(str.c_str(), size));
    }

    return *this;
}

Socket & Socket::operator>>(StringMap & map)
{
    map.clear();

    size_t size = receive(sizeof(size_t)).as<size_t>();

    for (size_t i = 0; i < size; i++)
    {
        std::string key;
        std::string val;
        *this >> key >> val;
        map.insert(std::make_pair(key, val));
    }

    return *this;
}

Socket & Socket::operator<<(const StringMap & map)
{
    size_t size = map.size();
    send(Bytes::from(size));

    if (size > 0)
    {
        for (StringMap::const_iterator it = map.begin(); it != map.end(); it++)
        {
            *this << it->first << it->second;
        }
    }

    return *this;
}

void Socket::act()
{
    assert(m_expecting <= m_buffer.size());

    if (m_expecting > 0)
    {
        char * buffer = &m_buffer[m_buffer.size() - m_expecting];
        int ret = ::recv(m_sockFd, buffer, m_expecting, 0);

        if (ret == 0)
        {
            throw std::runtime_error("connection closed by remote during recv");
        }
        else if (ret == -1)
        {
            THROW(std::runtime_error) << "recv returned error: " << Detail::Error::lastError();
        }
        else
        {
            m_expecting -= ret;
        }

        if (m_expecting == 0)
        {
            m_dataReceived(&m_buffer[0]);
        }
    }
}

auto Socket::nativeHandle() -> int const
{
    return m_sockFd;
}
