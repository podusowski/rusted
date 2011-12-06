#pragma once

#include <stdexcept>
#include "Log.hpp"

// usage example: DEEPP_EXCEPTION(what << "something happened");
#define CAKE_DEPENDENCY_INJECTION_EXCEPTION(s) \
{ \
    std::stringstream what; \
    s; \
    DEEPP_LOG(what.str()); \
    throw Cake::DependencyInjection::Detail::Exception(what.str()); \
}

namespace Cake
{
namespace DependencyInjection 
{
namespace Detail
{

class Exception : public std::runtime_error
{
public:
    Exception(const std::string & what) : std::runtime_error(what)
    {
    }
};

}
}
}
