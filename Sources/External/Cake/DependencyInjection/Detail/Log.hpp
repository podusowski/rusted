#pragma once

#include <string>
#include <cstdlib>

#ifdef __GNUG__
    #include <cxxabi.h>
#endif

#ifdef DEEPP_ENABLE_LOG
    #include <iostream>
    #define DEEPP_LOG(s) std::cerr << s << std::endl;
#else
    #define DEEPP_LOG(s) ;
#endif

#define CAKE_DEPENDENCY_INJECTION_TYPENAME(type) Cake::DependencyInjection::Detail::demangle(typeid(type))

namespace Cake
{
namespace DependencyInjection
{
namespace Detail
{

inline std::string demangle(const std::type_info & type)
{
#ifdef __GNUG__
    int status;
    char * realname = abi::__cxa_demangle(type.name(), 0, 0, &status);
    std::string result(realname);
    free(realname);

    return result;
#else
    return std::string("<") + type.name() + std::string(">");
#endif
}

}
}
}
