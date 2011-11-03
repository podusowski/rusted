#pragma once

#include <boost/optional.hpp>

namespace Cake
{
namespace Networking
{
namespace Detail
{

class SockFdGuard
{
public:
    SockFdGuard()
    {
    }

    SockFdGuard(int sockFd) :
        m_sockFd(sockFd)
    {
    }

    SockFdGuard & operator=(int sockFd)
    {
        m_sockFd = sockFd;
        return *this;
    }

    bool operator==(int sockFd)
    {
        return *m_sockFd == sockFd;
    }

    int release()
    {
        int ret = *m_sockFd;
        m_sockFd = boost::none;
        return ret;
    }

    int operator*()
    {
        return *m_sockFd;
    }

    ~SockFdGuard()
    {
        if (m_sockFd)
        {
            close(*m_sockFd);
        }
    }

private:
    boost::optional<int> m_sockFd;
};

}
}
}
