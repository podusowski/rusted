#pragma once

#include "GlobalRegistry.hpp"

namespace Cake
{
namespace DependencyInjection
{

class Registry
{
public:
    template<typename InterfaceType> Interface<InterfaceType> & forInterface()
    {
        return GlobalRegistry::instance().createInterface<InterfaceType>();
    }

    ~Registry()
    {
        GlobalRegistry::instance().clear();
    }
};

}
}
