#pragma once

#include <memory>

#include "Detail/NullType.hpp"
#include "Factory.hpp"

namespace Cake
{
namespace DependencyInjection
{

template<typename Interface, typename Implementation>
class GenericFactory0 : public Factory<Interface>
{
public:
    virtual std::shared_ptr<Interface> create()
    {
        return std::shared_ptr<Interface>(new Implementation());
    }
};

template<typename Interface, typename Implementation, typename Arg1>
class GenericFactory1 : public Factory<Interface, Arg1>
{
public:
    virtual std::shared_ptr<Interface> create(Arg1 arg1)
    {
        return std::shared_ptr<Interface>(new Implementation(arg1));
    }
};

}
}
