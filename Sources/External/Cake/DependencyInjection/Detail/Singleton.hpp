#pragma once

#include <boost/shared_ptr.hpp>

namespace Cake 
{
namespace DependencyInjection
{
namespace Detail
{

template<typename T> class Singleton
{
public:
    static T & instance()
    {
        static T instance;
        return instance;
    }
};

}
}
}
