#pragma once

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
    SocketDescriptor(int descriptor) :
        m_descriptor(descriptor)
    {
    }

    SocketDescriptor(int descriptor, std::string filename) :
        m_descriptor(descriptor),
        m_filename(filename)
    {
    }

    ~SocketDescriptor()
    {
        if (m_descriptor)
        {
            close(*m_descriptor);
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

    int operator*()
    {
        return *m_descriptor;
    }

private:
    boost::optional<int> m_descriptor;
    boost::optional<std::string> m_filename;
};

}
}
}
