#pragma once

#include <memory>

#include "Registry.hpp"
#include "Interface.hpp"
#include "Detail/NullType.hpp"

namespace Cake
{
namespace DependencyInjection
{

template<typename InterfaceType, typename Arg1 = Detail::NullType> class Inject
{
public:
    Inject()
    {
        std::shared_ptr<Detail::CycleGuard> g = createCycleGuard();
        m_object = factory<Factory<InterfaceType> >().create();
    }

    Inject(Arg1 arg1)
    {
        std::shared_ptr<Detail::CycleGuard> g = createCycleGuard();
        m_object = factory<Factory<InterfaceType, Arg1> >().create(arg1);
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
    Interface<InterfaceType> & interface()
    {
        return GlobalRegistry::instance().findInterface<InterfaceType>();
    }

    std::shared_ptr<Detail::CycleGuard> createCycleGuard()
    {
        return interface().createCycleGuard();
    }

    template<typename Factory> Factory & factory()
    {
        Interface<InterfaceType> & iface = GlobalRegistry::instance().findInterface<InterfaceType>();

        try
        {
            Factory & factory = dynamic_cast<Factory&>(*iface.get_factory());
            return factory;
        }
        catch (const std::bad_cast &)
        {
            CAKE_DEPENDENCY_INJECTION_EXCEPTION(what << "inject declaration mismatch: inject type: " 
                                                     << CAKE_DEPENDENCY_INJECTION_TYPENAME(*this) 
                                                     << ", registered factory: " 
                                                     << iface.get_factory()->describe());
        }
    }

    std::shared_ptr<InterfaceType> m_object;
};

}
}
