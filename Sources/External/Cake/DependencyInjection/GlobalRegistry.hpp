#pragma once

#include <map>
#include <sstream>
#include <stdexcept>
#include <memory>

#include "Interface.hpp"
#include "Detail/Singleton.hpp"
#include "Detail/Log.hpp"
#include "Detail/Exception.hpp"

namespace Cake 
{
namespace DependencyInjection
{

class Registry : public Detail::Singleton<Registry>
{
public:
    template<typename InterfaceType> Interface<InterfaceType> & create_interface()
    {
        interface_map::iterator it = m_interfaces.find(&typeid(InterfaceType));
        if (it == m_interfaces.end())
        {
            std::shared_ptr<IInterface> i(new Interface<InterfaceType>());
            m_interfaces.insert(std::make_pair(&typeid(InterfaceType), i));
            return dynamic_cast<Interface<InterfaceType>&>(*i);
        }
        else
        {
            CAKE_DEPENDENCY_INJECTION_EXCEPTION(what << "interface " << CAKE_DEPENDENCY_INJECTION_TYPENAME(InterfaceType) << " is already registered");
        }
    }

    template<typename InterfaceType> Interface<InterfaceType> & find_interface()
    {
        interface_map::iterator it = m_interfaces.find(&typeid(InterfaceType)); 
        if (it == m_interfaces.end())
        {
            CAKE_DEPENDENCY_INJECTION_EXCEPTION(what << "can't inject " << CAKE_DEPENDENCY_INJECTION_TYPENAME(InterfaceType) << " because no such object is registered");
        }
        return dynamic_cast<Interface<InterfaceType>&>(*it->second);
    }

    void clear()
    {
        m_interfaces.clear();
    }

private:
    typedef std::map<const std::type_info *, std::shared_ptr<IInterface> > interface_map;

    interface_map m_interfaces;
};

template<typename InterfaceType> Interface<InterfaceType> & forInterface()
{
    return Registry::instance().create_interface<InterfaceType>();
}

template<typename InterfaceType, typename Implementation> void use()
{
    forInterface<InterfaceType>().use<Implementation>();
}

inline void clear()
{
    Registry::instance().clear();
}

}
}
