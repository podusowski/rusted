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

#define CAKE_LOG_STREAM std::cout

#define CAKE_LOG_INFO std::cout << "INF/" << __FILE__ << ":" << __LINE__ << ": "
#define CAKE_LOG_ERR  std::cout << "ERR/" << __FILE__ << ":" << __LINE__ << ": "
#define CAKE_LOG_WARN std::cout << "WRN/" << __FILE__ << ":" << __LINE__ << ": "
