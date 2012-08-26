#pragma once

#include <sstream>
#include "ILogStream.hpp"

namespace Cake
{
namespace Diagnostics
{

class LogMessage
{
public:
    LogMessage(ILogStream & stream, const std::string & header);
    LogMessage(const LogMessage & other);
    ~LogMessage();
    
    template<typename T> LogMessage & operator << (const T & value)
    {
        m_ss << value;
        return *this;
    }

    template<typename T> LogMessage & operator << (T & value)
    {
        m_ss << value;
        return *this;
    }

private:
    void flush();
    void printMessage();

    ILogStream & m_stream;
    std::string m_header;
    std::stringstream m_ss;
};

}
}
