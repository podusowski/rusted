#include <cxxabi.h>

#include "Logger.hpp"

using namespace Cake::Diagnostics::Logger;

Logger::Logger()
{
}

Logger & Logger::getSingleton()
{
    static Logger logger;
    return logger;
}

std::string Logger::demangle(const std::string & name)
{
    int status;
    char * realname = abi::__cxa_demangle(name.c_str(), 0, 0, &status);
    std::string result(realname);
    free(realname);

    return result;
}
