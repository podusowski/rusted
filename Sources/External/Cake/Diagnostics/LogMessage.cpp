#include <iostream>

#include "LogMessage.hpp"

using namespace Cake::Diagnostics;

LogMessage::LogMessage(const std::string & header) :
    m_header(header)
{
}

LogMessage::LogMessage(const LogMessage & other) : 
    m_header(other.m_header),
    m_stream()
{
    flush();
}

LogMessage::~LogMessage()
{
    flush();
}
    
void LogMessage::flush()
{
    if (!m_stream.str().empty())
    {
        printMessage();
    }
}

void LogMessage::printMessage()
{
    std::stringstream out;
    out << m_header;

    std::string message = m_stream.str();

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

