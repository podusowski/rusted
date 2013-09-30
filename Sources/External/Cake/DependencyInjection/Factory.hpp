#pragma once

#include <memory>

#include "Detail/NullType.hpp"
#include "Detail/Exception.hpp"

namespace Cake
{
namespace DependencyInjection
{

class IFactory 
{
public:
    virtual std::string describe()
    {
        return "abstract_factory";
    }

    virtual ~IFactory() {}
};

template<typename Interface, typename Arg1 = Detail::NullType>
class Factory : public IFactory 
{
public:
    virtual std::shared_ptr<Interface> create()
    {
        CAKE_DEPENDENCY_INJECTION_EXCEPTION(what << "this fatory doesn't implement this parameters (void)");
    }

    virtual std::shared_ptr<Interface> create(Arg1 arg1)
    {
        CAKE_DEPENDENCY_INJECTION_EXCEPTION(what << "this fatory doesn't implement this parameters (" << CAKE_DEPENDENCY_INJECTION_TYPENAME(Arg1) << ")");
    }

    virtual std::string describe()
    {
        std::stringstream ss;
        ss << CAKE_DEPENDENCY_INJECTION_TYPENAME(*this) << " (" << CAKE_DEPENDENCY_INJECTION_TYPENAME(Arg1) << ")";
        return ss.str();
    }
};

}
}
