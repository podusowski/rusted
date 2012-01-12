#pragma once

#include <iostream>
#include <cstdlib>

#include "Cake/DependencyInjection/Inject.hpp"

namespace Cake
{
namespace Diagnostics
{
namespace Logger
{

enum LogType
{
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class Logger
{
public:
    static Logger & getSingleton();
    std::string demangle(const std::string & name);

private:
    Logger();
    Logger(const Logger &) {}
};

}
}
}

#ifndef LOF_INFO

#define LOG_INFO  std::cout << "\n\033[1;34mINF\033[0m/" << __FILE__ << ":" << __LINE__ << ": "
#define LOG_ERR   std::cout << "\n\033[1;31mERR\033[0m/" << __FILE__ << ":" << __LINE__ << ": "
#define LOG_WARN  std::cout << "\n\033[1;33mWRN\033[0m/" << __FILE__ << ":" << __LINE__ << ": "
#define LOG_DEBUG std::cout << "\n\033[1;37mDBG\033[0m/" << __FILE__ << ":" << __LINE__ << ": "

#define TYPENAME(t) Cake::Diagnostics::Logger::Logger::getSingleton().demangle(typeid(t).name())

#endif
