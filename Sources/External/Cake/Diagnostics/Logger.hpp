#pragma once

#include <map>
#include <cstdlib>

#include "LogMessage.hpp"
#include "ILogStream.hpp"
#include "Cake/Configuration/Configuration.hpp"
#include "Cake/Threading/Mutex.hpp"

namespace Cake
{
namespace Diagnostics
{

class DummyLogStream
{
public:
    template<typename T> DummyLogStream & operator << (const T &)
    {
        return *this;
    }
};

enum LogLevel
{
    LogLevel_DEBUG,
    LogLevel_INFO,
    LogLevel_WARNING,
    LogLevel_ERROR
};

class Logger : public ILogStream
{
public:
    static Logger & getInstance();
    void setAppName(const std::string &);
    std::string demangle(const std::string & name);

    LogMessage log(LogLevel level,
                       const std::string & file,
                       unsigned line);

    DummyLogStream dummyLog()
    {
        return DummyLogStream();
    }

    void flush(const std::string &);

private:
    Logger();
    Logger(const Logger &) {}

    std::string generateHeader(LogLevel, const std::string & file, unsigned line);
    std::string generateShortPath(std::string path) const;
    std::string generateTime() const;
    std::string generateThread() const;
    std::string generateLevel(LogLevel level) const;

    std::string m_appName;
    Cake::Threading::Mutex m_mutex;
};

}
}

#define LOG(level) Cake::Diagnostics::Logger::getInstance().log(level, __FILE__, __LINE__)
#define DUMMY_LOG Cake::Diagnostics::Logger::getInstance().dummyLog()

#ifdef NDEBUG
    #define LOG_DEBUG DUMMY_LOG
#else
    #define LOG_DEBUG LOG(Cake::Diagnostics::LogLevel_DEBUG)
#endif

#define LOG_INFO LOG(Cake::Diagnostics::LogLevel_INFO)
#define LOG_WARN LOG(Cake::Diagnostics::LogLevel_WARNING)
#define LOG_ERR LOG(Cake::Diagnostics::LogLevel_ERROR)

#define TYPENAME(t) Cake::Diagnostics::Logger::Logger::getInstance().demangle(typeid(t).name())

