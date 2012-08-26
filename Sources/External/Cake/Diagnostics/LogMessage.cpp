#include <iostream>

#include "LogMessage.hpp"

using namespace Cake::Diagnostics;

LogMessage::LogMessage(ILogStream & stream, const std::string & header) :
    m_stream(stream),
    m_header(header)
{
}

LogMessage::LogMessage(const LogMessage & other) : 
    m_stream(other.m_stream),
    m_header(other.m_header)
{
    flush();
}

LogMessage::~LogMessage()
{
    flush();
}
    
void LogMessage::flush()
{
    if (!m_ss.str().empty())
    {
        printMessage();
    }
}

void LogMessage::printMessage()
{
    std::stringstream out;
    out << m_header;

    std::string message = m_ss.str();

    for (std::string::iterator it = message.begin(); it != message.end(); it++)
    {
        if (*it == '\n')
        {
            out << '\n' << m_header;
        }
        else
        {
            out << *it;
        }
    }
    std::cerr << out.str() << '\n';
}

