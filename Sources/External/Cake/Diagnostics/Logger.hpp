#pragma once

#include <iostream>
#include <cxxabi.h>
#include <cstdlib>

namespace Cake
{
namespace Diagnostics
{
namespace Logger
{
namespace Detail
{

class NullStream
{
public:
    template <class T> NullStream & operator << (const T &)
    {
        return *this;
    }
};

inline std::string demangle(const std::string & name)
{
    int status;
    char * realname = abi::__cxa_demangle(name.c_str(), 0, 0, &status);
    std::string result(realname);
    free(realname);

    return result;
}

}
}
}
}

#ifndef LOF_INFO

#define LOG_INFO  std::cout << "\n\033[1;34mINF\033[0m/" << __FILE__ << ":" << __LINE__ << ": "
#define LOG_ERR   std::cout << "\n\033[1;31mERR\033[0m/" << __FILE__ << ":" << __LINE__ << ": "
#define LOG_WARN  std::cout << "\n\033[1;33mWRN\033[0m/" << __FILE__ << ":" << __LINE__ << ": "
#define LOG_DEBUG std::cout << "\n\033[1;37mDBG\033[0m/" << __FILE__ << ":" << __LINE__ << ": "

#define TYPENAME(t) Cake::Diagnostics::Logger::Detail::demangle(typeid(t).name())

#endif
