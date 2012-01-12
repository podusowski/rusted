#pragma once

#include <map>
#include <iostream>
#include <cstdlib>

#include "LogMessage.hpp"
#include "Cake/DependencyInjection/Inject.hpp"
#include "Cake/Configuration/Configuration.hpp"

namespace Cake
{
namespace Diagnostics
{

enum LogLevel
{
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class Logger
{
public:
    static Logger & getInstance();
    void setAppName(const std::string &);
    std::string demangle(const std::string & name);

    LogMessage log(LogLevel level, 
                       const std::string & file, 
                       unsigned line);

private:
    Logger();
    Logger(const Logger &) {}

    std::string generateHeader(LogLevel, const std::string & file, unsigned line);

    std::map<LogLevel, std::string> m_banners;
    std::string m_appName;
};

}
}

#define LOG(level) Cake::Diagnostics::Logger::getInstance().log(level, __FILE__, __LINE__)

#define LOG_DEBUG LOG(Cake::Diagnostics::DEBUG)
#define LOG_INFO LOG(Cake::Diagnostics::INFO)
#define LOG_WARN LOG(Cake::Diagnostics::WARNING)
#define LOG_ERR LOG(Cake::Diagnostics::ERROR)

#define TYPENAME(t) Cake::Diagnostics::Logger::Logger::getInstance().demangle(typeid(t).name())

