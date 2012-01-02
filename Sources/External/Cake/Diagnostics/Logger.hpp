#pragma once

#include <iostream>

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

}
}
}
}

#ifndef LOF_INFO

#define LOG_INFO  std::cout << "\033[1;34mINF\033[0m/" << __FILE__ << ":" << __LINE__ << ": "
#define LOG_ERR   std::cout << "\033[1;31mERR\033[0m/" << __FILE__ << ":" << __LINE__ << ": "
#define LOG_WARN  std::cout << "\033[1;33mWRN\033[0m/" << __FILE__ << ":" << __LINE__ << ": "
#define LOG_DEBUG std::cout << "\033[1;37mDBG\033[0m/" << __FILE__ << ":" << __LINE__ << ": "

#endif
