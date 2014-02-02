#pragma once

#include <memory>

#include "Detail/Log.hpp"
#include "AbstractFactory.hpp"
#include "GenericFactory.hpp"
#include "InstanceFactory.hpp"
#include "Detail/Exception.hpp"

namespace Cake
{
namespace DependencyInjection
{

namespace Detail
{
    class CycleGuard
    {
    public:
        CycleGuard(unsigned & counter, const std::type_info & type) : m_counter(counter)
        {
            if (m_counter++ > 0)
            {
                CAKE_DEPENDENCY_INJECTION_EXCEPTION(what << "cycle detected while injecting " << CAKE_DEPENDENCY_INJECTION_TYPENAME(type));
            }
        }

        ~CycleGuard()
        {
            --m_counter;
        }

    private:
        unsigned & m_counter;
    };
}

class IInterface
{
public:
    virtual const std::type_info & getTypeInfo() = 0;
    virtual ~IInterface() {}
};

template<typename InterfaceType> class Interface : public IInterface
{
public:
    Interface() : m_cycleCounter(0)
    {
    }

    virtual const std::type_info & getTypeInfo()
    {
        return typeid(InterfaceType);
    }

    template<typename Implementation> void use()
    {
        m_factory = std::make_shared<GenericFactory<InterfaceType, Implementation>>();
    }

    void use(std::shared_ptr<InterfaceType> instance)
    {
        m_factory = std::make_shared<InstanceFactory<InterfaceType>>(instance);
    }

    template<typename FactoryType> void useFactory()
    {
        // TODO: make it lazy, otherwise we can get troubles when factory is
        // using inject by itself
        m_factory = std::make_shared<FactoryType>();
    }

    void useFactory(std::shared_ptr<IFactory> factory)
    {
        m_factory = factory;
    }

    std::shared_ptr<IFactory> getFactory()
    {
        if (!m_factory.get())
        {
            CAKE_DEPENDENCY_INJECTION_EXCEPTION(what << CAKE_DEPENDENCY_INJECTION_TYPENAME(InterfaceType) << " has no factory");
        }
        return m_factory;
    }

    std::shared_ptr<Detail::CycleGuard> createCycleGuard()
    {
        // TODO: this can be more lightweight
        return std::make_shared<Detail::CycleGuard>(m_cycleCounter, typeid(InterfaceType));
    }

private:
    std::shared_ptr<IFactory> m_factory;
    unsigned m_cycleCounter;
};

}
}
