#pragma once

#include <boost/shared_ptr.hpp>

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
        boost::shared_ptr<Detail::CycleGuard> g = create_cycle_guard();
        _object = find_factory<Factory<InterfaceType> >().create();
    }

    Inject(Arg1 arg1)
    {
        boost::shared_ptr<Detail::CycleGuard> g = create_cycle_guard();
        _object = find_factory<Factory<InterfaceType, Arg1> >().create(arg1);
    }

    InterfaceType * operator->()
    {
        return _object.get();
    }

    InterfaceType & operator*()
    {
        return *_object;
    }

private:
    Interface<InterfaceType> & find_interface()
    {
        return Registry::instance().find_interface<InterfaceType>();
    }

    boost::shared_ptr<Detail::CycleGuard> create_cycle_guard()
    {
        return find_interface().create_cycle_guard();
    }

    template<typename Factory> Factory & find_factory()
    {
        Interface<InterfaceType> & iface = Registry::instance().find_interface<InterfaceType>();
        try
        {
            Factory & factory = dynamic_cast<Factory&>(*iface.get_factory());
            return factory;
        }
        catch (std::bad_cast &)
        {
            CAKE_DEPENDENCY_INJECTION_EXCEPTION(what << "inject declaration mismatch: inject type: " 
                                 << CAKE_DEPENDENCY_INJECTION_TYPENAME(*this) 
                                 << ", registered factory: " 
                                 << iface.get_factory()->describe());
        }
    }

    boost::shared_ptr<InterfaceType> _object;
};

}
}
