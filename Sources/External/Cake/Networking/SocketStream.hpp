#pragma once

#include <map>

namespace Cake 
{
namespace Networking
{

class SocketStream
{
public:
    typedef std::map<std::string, std::string> StringMap;

    SocketStream(int socket);

    void send(const void *, size_t);
    void receive(void *, size_t);

    SocketStream & operator>>(unsigned &);
    SocketStream & operator<<(unsigned);

    SocketStream & operator>>(std::string &);
    SocketStream & operator<<(const std::string &);

    SocketStream & operator>>(StringMap &);
    SocketStream & operator<<(const StringMap &);

private:
    int m_socket;
};

}
}
