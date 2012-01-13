#pragma once

#include <sstream>

namespace Cake
{
namespace Diagnostics
{

class LogMessage
{
public:
    LogMessage(const std::string & header);
    LogMessage(const LogMessage & other);
    ~LogMessage();
    
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
    void flush();
    void printMessage();

    std::string m_header;
    std::stringstream m_stream;
};

}
}
