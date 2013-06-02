#pragma once

#include <map>
#include <cstdlib>

#include "LogMessage.hpp"
#include "ILogStream.hpp"
#include "Cake/DependencyInjection/Inject.hpp"
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
    DEBUG,
    INFO,
    WARNING,
    ERROR
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

    std::map<LogLevel, std::string> m_banners;
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
    #define LOG_DEBUG LOG(Cake::Diagnostics::DEBUG)
#endif

#define LOG_INFO LOG(Cake::Diagnostics::INFO)
#define LOG_WARN LOG(Cake::Diagnostics::WARNING)
#define LOG_ERR LOG(Cake::Diagnostics::ERROR)

#define TYPENAME(t) Cake::Diagnostics::Logger::Logger::getInstance().demangle(typeid(t).name())

