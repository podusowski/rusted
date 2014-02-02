#pragma once

#include <memory>

#include "GlobalRegistry.hpp"
#include "Interface.hpp"

namespace Cake
{
namespace DependencyInjection
{

template<typename InterfaceType, typename... Args> class Inject
{
public:
    Inject(Args... args)
    {
        auto g = createCycleGuard();
        m_object = findFactory<AbstractFactory<InterfaceType, Args...>>().create(args...);
    }

    InterfaceType * operator->()
    {
        return m_object.get();
    }

    const InterfaceType * operator->() const
    {
        return m_object.get();
    }

    InterfaceType & operator*()
    {
        return *m_object;
    }

private:
    Interface<InterfaceType> & findInterface() const
    {
        return GlobalRegistry::instance().findInterface<InterfaceType>();
    }

    std::shared_ptr<Detail::CycleGuard> createCycleGuard() const
    {
        return findInterface().createCycleGuard();
    }

    template<typename FactoryType> FactoryType & findFactory() const
    {
        auto & i = findInterface();

        try
        {
            return dynamic_cast<FactoryType&>(*i.getFactory());
        }
        catch (const std::bad_cast &)
        {
            CAKE_DEPENDENCY_INJECTION_EXCEPTION(what << "inject declaration mismatch: inject type: "
                                                     << CAKE_DEPENDENCY_INJECTION_TYPENAME(*this)
                                                     << ", registered factory: "
                                                     << i.getFactory()->describe());
        }
    }

    std::shared_ptr<InterfaceType> m_object;
};

}
}
