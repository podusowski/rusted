#pragma once

#include "Detail/Log.hpp"
#include "Factory.hpp"
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
    virtual const std::type_info & get_interface() = 0;
    virtual ~IInterface() {}
};

template<typename InterfaceType> class Interface : public IInterface
{
public:
    Interface() : m_cycle_counter(0)
    {
    }

    virtual const std::type_info & get_interface()
    {
        return typeid(InterfaceType);
    }

    template<typename Implementation> void use()
    {
        m_factory.reset(new GenericFactory0<InterfaceType, Implementation>());
    }

    void use(boost::shared_ptr<InterfaceType> instance)
    {
        m_factory = boost::shared_ptr<IFactory>(new instance_factory<InterfaceType>(instance));
    }

    template<typename Factory> void use_factory()
    {
        // TODO: make it lazy, otherwise we can get troubles when factory is
        // using inject by itself
        m_factory = boost::shared_ptr<IFactory>(new Factory()); 
    }

    boost::shared_ptr<IFactory> get_factory()
    {
        if (!m_factory.get())
        {
            CAKE_DEPENDENCY_INJECTION_EXCEPTION(what << CAKE_DEPENDENCY_INJECTION_TYPENAME(InterfaceType) << " has no factory");
        }
        return m_factory;
    }

    boost::shared_ptr<Detail::CycleGuard> create_cycle_guard()
    {
        return boost::shared_ptr<Detail::CycleGuard>(new Detail::CycleGuard(m_cycle_counter, typeid(InterfaceType)));
    }

private:
    boost::shared_ptr<IFactory> m_factory;
    unsigned m_cycle_counter;
};

}
}
