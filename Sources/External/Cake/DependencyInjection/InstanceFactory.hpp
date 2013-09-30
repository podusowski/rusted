#pragma once

#include <std/shared_ptr.hpp>

#include "Factory.hpp"

namespace Cake 
{
namespace DependencyInjection
{

template<typename Interface>
class instance_factory : public Factory<Interface>
{
public:
    instance_factory(std::shared_ptr<Interface> instance) :
        m_instance(instance)
    {
    }

    virtual std::shared_ptr<Interface> create()
    {
        return m_instance;
    }

private:
    std::shared_ptr<Interface> m_instance;
};

}
}
