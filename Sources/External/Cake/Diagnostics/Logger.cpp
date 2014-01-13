#include <iostream>
#include <cxxabi.h>
#include <iomanip>
#include <chrono>

#include "Cake/Threading/Thread.hpp"
#include "Cake/Threading/ScopedLock.hpp"
#include "Logger.hpp"

using namespace Cake::Diagnostics;

Logger::Logger() : m_appName("undef")
{
}

Logger & Logger::getInstance()
{
    static Logger logger;
    return logger;
}

void Logger::setAppName(const std::string & appname)
{
    m_appName = appname;
}

std::string Logger::demangle(const std::string & name)
{
    int status;
    char * realname = abi::__cxa_demangle(name.c_str(), 0, 0, &status);
    std::string result(realname);
    free(realname);

    return result;
}

LogMessage Logger::log(LogLevel level,
                       const std::string & file, 
                       unsigned line)
{
    return LogMessage(*this, generateHeader(level, file, line));
}

void Logger::flush(const std::string & s)
{
    Cake::Threading::ScopedLock lock(m_mutex);
    std::cerr << s;
}

std::string Logger::generateHeader(LogLevel level, const std::string & file, unsigned line)
{
    std::stringstream fileWithLine;
    fileWithLine << generateShortPath(file) << ":" << line;

    std::stringstream ss;
    ss << generateTime() << std::setw(10) << std::setiosflags(std::ios::right)
       << m_appName
       << "(" << generateThread() << ") "
       << std::setw(34) << std::setiosflags(std::ios::right) << fileWithLine.str()
       << " "
       << generateLevel(level) << " ";

    return ss.str();
}

std::string Logger::generateShortPath(std::string path) const
{
    const unsigned MAX_LENGTH = 30;

    if (path.length() > MAX_LENGTH)
    {
        return ".." + path.substr(path.length() - MAX_LENGTH + 2, MAX_LENGTH - 2);
    }

    return path;
}

std::string Logger::generateTime() const
{
    using namespace std::chrono;

    static auto firstMessageTime = high_resolution_clock::now();

    std::stringstream ss;
    auto now = high_resolution_clock::now();
    auto totalMs = duration_cast<milliseconds>(now - firstMessageTime).count();
    int seconds = totalMs / 1000;
    int ms = totalMs - (seconds * 1000);
    ss << std::setfill('0') << std::setw(4) << seconds << "." << std::setw(3) << ms;

    return ss.str();
}

std::string Logger::generateThread() const
{
    std::stringstream ss;
    ss << "thread:" << Cake::Threading::Thread::self();
    return ss.str();
}

std::string Logger::generateLevel(LogLevel level) const
{
    switch (level)
    {
        case LogLevel_DEBUG:
            return "DBG";

        case LogLevel_INFO:
            return "INF";

        case LogLevel_WARNING:
            return "WRN";

        case LogLevel_ERROR:
            return "ERR";

        default:
            return "UNDEF";
    }
}
