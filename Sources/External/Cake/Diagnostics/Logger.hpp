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
#define LOG_INFO std::cout << "INF/" << __FILE__ << ":" << __LINE__ << ": "
#define LOG_ERR  std::cout << "ERR/" << __FILE__ << ":" << __LINE__ << ": "
#define LOG_WARN std::cout << "WRN/" << __FILE__ << ":" << __LINE__ << ": "
#endif
