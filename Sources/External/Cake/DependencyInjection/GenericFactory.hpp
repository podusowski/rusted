#pragma once

#include <memory>

#include "AbstractFactory.hpp"

namespace Cake
{
namespace DependencyInjection
{

template<typename Interface, typename Implementation, typename... Args> class GenericFactory : public AbstractFactory<Interface, Args...>
{
public:
    std::shared_ptr<Interface> create(Args... args)
    {
        return std::make_shared<Implementation>(args...);
    }
};

}
}
