#pragma once

#include <boost/shared_ptr.hpp>

#include "Detail/NullType.hpp"
#include "Factory.hpp"

namespace Cake
{
namespace DependencyInjection
{

template<typename Interface, typename Implementation>
class generic_factory0 : public Factory<Interface>
{
public:
    virtual boost::shared_ptr<Interface> create()
    {
        return boost::shared_ptr<Interface>(new Implementation());
    }
};

template<typename Interface, typename Implementation, typename Arg1>
class generic_factory1 : public Factory<Interface, Arg1>
{
public:
    virtual boost::shared_ptr<Interface> create(Arg1 arg1)
    {
        return boost::shared_ptr<Interface>(new Implementation(arg1));
    }
};

}
}
