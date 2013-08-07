#pragma once

#include <map>
#include <string>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>

namespace Cake
{
namespace Networking
{

class Socket : public boost::noncopyable
{
public:
    typedef std::map<std::string, std::string> StringMap;

    Socket(int sockFd);
    ~Socket();

    static boost::shared_ptr<Socket> connectToUnixSocket(const std::string & path);
    static boost::shared_ptr<Socket> connectToTcpSocket(const std::string & address, int port);

    void send(const void *, size_t);
    void receive(void *, size_t);

    Socket & operator>>(unsigned &);
    Socket & operator<<(unsigned);

    Socket & operator>>(std::string &);
    Socket & operator<<(const std::string &);

    Socket & operator>>(StringMap &);
    Socket & operator<<(const StringMap &);

private:
    int m_sockFd;
};

}
}

