#pragma once

#include <string>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>

#include "SocketStream.hpp"

namespace Cake 
{
namespace Networking
{

class Socket : public boost::noncopyable
{
public:
    Socket(int sockFd);
    ~Socket();

    static boost::shared_ptr<Socket> connectToUnixSocket(const std::string & path);
    static boost::shared_ptr<Socket> connectToTcpSocket(const std::string & address, int port);

    SocketStream & stream();

private:
    int m_sockFd;
    boost::scoped_ptr<SocketStream> m_stream;
};

}
}
