#pragma once

#include <sstream>

namespace Cake
{
namespace Diagnostics
{

class LogMessage
{
public:
    LogMessage(const std::string & header) :
        m_header(header)
    {
    }

    LogMessage(const LogMessage & other) : 
        m_header(other.m_header),
        m_stream()
    {
        flush();
    }

    ~LogMessage()
    {
        flush();
    }

    template<typename T> LogMessage & operator << (const T & value)
    {
        m_stream << value;
        return *this;
    }

    template<typename T> LogMessage & operator << (T & value)
    {
        m_stream << value;
        return *this;
    }


private:
    void flush()
    {
        if (!m_stream.str().empty())
        {
            std::cerr << m_header << m_stream.str() << "\n";
        }
    }

    std::string m_header;
    std::stringstream m_stream;
};

}
}
