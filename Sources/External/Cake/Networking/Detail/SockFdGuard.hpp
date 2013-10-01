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

#ifdef _WIN32
    typedef SOCKET DescriptorType;
#else
    typedef int DescriptorType;
#endif

    SockFdGuard()
    {
    }

    SockFdGuard(DescriptorType sockFd) :
        m_sockFd(sockFd)
    {
    }

    SockFdGuard & operator=(DescriptorType sockFd)
    {
        m_sockFd = sockFd;
        return *this;
    }

    bool operator==(DescriptorType sockFd)
    {
        return *m_sockFd == sockFd;
    }

    DescriptorType release()
    {
        auto ret = *m_sockFd;
        m_sockFd = boost::none;
        return ret;
    }

    DescriptorType operator*()
    {
        return *m_sockFd;
    }

    ~SockFdGuard()
    {
        if (m_sockFd)
        {
#ifdef _WIN32
            closesocket(*m_sockFd);
#else
            close(*m_sockFd);
#endif
        }
    }

private:
    boost::optional<DescriptorType> m_sockFd;
};

}
}
}
