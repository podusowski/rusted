#pragma once

#include <memory>

#include "Detail/Exception.hpp"

namespace Cake
{
namespace DependencyInjection
{

class IFactory
{
public:
    virtual std::string describe() = 0;
    virtual ~IFactory() {}
};

template<typename Interface, typename... Args> class AbstractFactory : public IFactory
{
public:
    virtual std::shared_ptr<Interface> create(Args...) = 0;

    virtual std::string describe()
    {
        std::stringstream ss;
        ss << CAKE_DEPENDENCY_INJECTION_TYPENAME(*this);
        return ss.str();
    }
};

}
}
