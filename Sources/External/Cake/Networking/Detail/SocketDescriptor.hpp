#pragma once

#ifdef _WIN32
    #include <windows.h>
#endif

#include <boost/optional.hpp>
#include <string>

namespace Cake
{
namespace Networking
{
namespace Detail
{

class SocketDescriptor
{
public:

#ifdef _WIN32
    typedef HANDLE DescriptorType;
#else
    typedef int DescriptorType;
#endif

    SocketDescriptor(DescriptorType descriptor) :
        m_descriptor(descriptor)
    {
    }

    SocketDescriptor(DescriptorType descriptor, std::string filename) :
        m_descriptor(descriptor),
        m_filename(filename)
    {
    }

    ~SocketDescriptor()
    {
        if (m_descriptor)
        {
#ifdef _WIN32
            CloseHandle(*m_descriptor);
#else
            close(*m_descriptor);
#endif
        }

        if (m_filename)
        {
            unlink(m_filename->c_str());
        }
    }

    SocketDescriptor(SocketDescriptor & rhs)
    {
        m_descriptor = rhs.m_descriptor;
        m_filename = rhs.m_filename;

        rhs.m_descriptor = boost::none;
        rhs.m_filename = boost::none;
    }

    DescriptorType operator*()
    {
        return *m_descriptor;
    }

private:
    boost::optional<DescriptorType> m_descriptor;
    boost::optional<std::string> m_filename;
};

}
}
}
