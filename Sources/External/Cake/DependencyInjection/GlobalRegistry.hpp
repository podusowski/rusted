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

class GlobalRegistry : public Detail::Singleton<GlobalRegistry>
{
public:
    template<typename InterfaceType> Interface<InterfaceType> & createInterface()
    {
        static_assert(std::is_polymorphic<InterfaceType>::value, "interface must be polymorphic");

        const auto it = m_interfaces.find(&typeid(InterfaceType));
        if (it == m_interfaces.end())
        {
            auto interface = std::make_shared<Interface<InterfaceType>>();
            m_interfaces.insert(std::make_pair(&typeid(InterfaceType), interface));
            return *interface;
        }
        else
        {
            CAKE_DEPENDENCY_INJECTION_EXCEPTION(what << "interface " << CAKE_DEPENDENCY_INJECTION_TYPENAME(InterfaceType) << " is already registered");
        }
    }

    template<typename InterfaceType> Interface<InterfaceType> & findInterface() const
    {
        const auto it = m_interfaces.find(&typeid(InterfaceType));
        if (it == m_interfaces.end())
        {
            CAKE_DEPENDENCY_INJECTION_EXCEPTION(what << "can't inject " << CAKE_DEPENDENCY_INJECTION_TYPENAME(InterfaceType) << " because no such object is registered");
        }
        // TODO: do we really need dynamic_cast here?
        return dynamic_cast<Interface<InterfaceType>&>(*it->second);
    }

    void clear()
    {
        m_interfaces.clear();
    }

private:
    std::map<const std::type_info *, std::shared_ptr<IInterface> > m_interfaces;
};

template<typename InterfaceType> Interface<InterfaceType> & forInterface()
{
    return GlobalRegistry::instance().createInterface<InterfaceType>();
}

template<typename InterfaceType, typename Implementation> void use()
{
    forInterface<InterfaceType>().template use<Implementation>();
}

inline void clear()
{
    GlobalRegistry::instance().clear();
}

}
}
