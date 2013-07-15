#include <stdexcept>
#include <cstring>
#include <sys/socket.h>
#include <errno.h>
#include <sstream>
#include <vector>

#include "SocketStream.hpp"

using namespace Cake::Networking;

SocketStream::SocketStream(int socket) : m_socket(socket)
{
}

void SocketStream::send(const void * buf, size_t size)
{
    int ret = ::send(m_socket, buf, size, MSG_NOSIGNAL);

    if (ret == -1)
    {
        std::stringstream ss;
        ss << "send: " << strerror(errno);
        throw std::runtime_error(ss.str());
    }
}

void SocketStream::receive(void * buf, size_t size)
{
    int ret = ::recv(m_socket, buf, size, 0);

    if (ret == 0)
    {
        throw std::runtime_error("connection closed by the peer during recv");
    }
    else if (ret == -1)
    {
        throw std::runtime_error("error during recv");
    }
    else if (ret != int(size))
    {
        throw std::runtime_error("received size doesn't match the expectation");
    }
}

SocketStream & SocketStream::operator>>(unsigned & value)
{
    receive(&value, sizeof(value));
    return *this;
}

SocketStream & SocketStream::operator<<(unsigned value)
{
    send(&value, sizeof(value));
    return *this;
}

SocketStream & SocketStream::operator>>(std::string & str)
{
    // TODO: we can operate on the string directly
    str.clear();
    
    size_t size;
    receive(&size, sizeof(size));

    if (size > 0)
    {
        if (size > 1024 * 1024) // 1MB
        {
            throw std::runtime_error("size of string exceeded 1MB, can't receive");
        }

        std::vector<char> buf(size + 1);
        buf.back() = 0;
        receive(&buf[0], size);
        str = &buf[0];
    }

    return *this;
}

SocketStream & SocketStream::operator<<(const std::string & str)
{
    size_t size = str.length();
    send(&size, sizeof(size));

    if (size > 0)
    {
        send(str.c_str(), size);
    }

    return *this;
}

SocketStream & SocketStream::operator>>(StringMap & map)
{
    map.clear();

    size_t size;
    receive(&size, sizeof(size));

    for (size_t i = 0; i < size; i++)
    {
        std::string key;
        std::string val;
        *this >> key >> val;
        map.insert(std::make_pair(key, val));
    }

    return *this;
}

SocketStream & SocketStream::operator<<(const StringMap & map)
{
    size_t size = map.size();
    send(&size, sizeof(size));

    if (size > 0)
    {
        for (StringMap::const_iterator it = map.begin(); it != map.end(); it++)
        {
            *this << it->first << it->second;
        }
    }

    return *this;
}
