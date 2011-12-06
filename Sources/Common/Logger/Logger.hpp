#pragma once

#include <cxxabi.h>

#include <boost/lexical_cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>
#include <string>
#include <sstream>

namespace Common
{
namespace Logger 
{

enum LogType
{
    LOGTYPE_INFO,
    LOGTYPE_WARN,
    LOGTYPE_ERR,
    LOGTYPE_SCT,
    LOGTYPE_TRAFFIC
};

class LoggerStream 
{
public:
    LoggerStream(LogType level) : m_level(level), m_header(true)
    {
    }

    template<typename T> LoggerStream & operator <<(const T & s)
    {
        std::stringstream ss;
        ss << s;
        printString(ss.str());
        return *this; 
    }

    void setPrefix(const std::string & prefix)
    {
        m_prefix = prefix;

        const size_t maxLength = 30;

        if (m_prefix.length() > maxLength)
        {
            m_prefix = ".." + m_prefix.substr(m_prefix.length() - maxLength + 2, maxLength - 2);
        }
        else
        {
            size_t oldLength = m_prefix.length();
            for (size_t i = 0; i < maxLength - oldLength; i++) m_prefix = " " + m_prefix;
        }
    }

    LoggerStream & setHeader(bool value)
    {
        m_header = value;
        return *this;
    }

private:
    void printString(const std::string & s)
    {
        if (m_header)
            printHeader();

        for (std::string::const_iterator it = s.begin(); it != s.end(); it++)
        {
            std::cout << *it;
            if (*it == '\n')
            {
                if (it + 1 != s.end())
                { 
                   printHeader();
                }
                setHeader(true);
            }
            else
            {
                setHeader(false);
            }
        }
    }

    void printHeader()
    {
        boost::posix_time::ptime t(boost::date_time::second_clock<boost::posix_time::ptime>::local_time());
        std::cout << t << " " << m_prefix << " ";

        switch (m_level)
        {
        case LOGTYPE_INFO:
            std::cout << "\033[1;34mINF\033[0m "; break;
        case LOGTYPE_WARN:
            std::cout << "\033[1;33mWRN\033[0m "; break;
        case LOGTYPE_ERR:
            std::cout << "\033[1;31mERR\033[0m "; break;
        case LOGTYPE_SCT:
            std::cout << "\033[1;32mSCT\033[0m "; break;
        case LOGTYPE_TRAFFIC:
            std::cout << "\033[1;37mTRF\033[0m "; break;
        }

        setHeader(false);
    }

    LogType m_level;
    std::string m_prefix;
    bool m_header;
};

static LoggerStream info(LOGTYPE_INFO);
static LoggerStream warn(LOGTYPE_WARN);
static LoggerStream err(LOGTYPE_ERR);
static LoggerStream sct(LOGTYPE_SCT);
static LoggerStream traffic(LOGTYPE_TRAFFIC);

inline std::string demangleType(const std::string & name)
{
    int status;
    char * realname = abi::__cxa_demangle(name.c_str(), 0, 0, &status);
    std::string result(realname);
    free(realname);

    return result;
}

}
}

#define LOG_INFO Common::Logger::info.setPrefix(std::string(__FILE__) + ":" + boost::lexical_cast<std::string>(__LINE__)); Common::Logger::info.setHeader(true)
#define LOG_WARN Common::Logger::warn.setPrefix(std::string(__FILE__) + ":" + boost::lexical_cast<std::string>(__LINE__)); Common::Logger::warn.setHeader(true)
#define LOG_ERR  Common::Logger::err .setPrefix(std::string(__FILE__) + ":" + boost::lexical_cast<std::string>(__LINE__)); Common::Logger::err .setHeader(true)
#define LOG_SCT  Common::Logger::sct .setPrefix(std::string(__FILE__) + ":" + boost::lexical_cast<std::string>(__LINE__)); Common::Logger::sct .setHeader(true)
#define LOG_TRAFFIC Common::Logger::traffic .setPrefix(std::string(__FILE__) + ":" + boost::lexical_cast<std::string>(__LINE__)); Common::Logger::traffic .setHeader(true)

#define TYPENAME(t) Common::Logger::demangleType(typeid(t).name())
