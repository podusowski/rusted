#pragma once

#include <vector>
#include <boost/shared_ptr.hpp>

#include "Object/IObject.hpp"
#include "DataBase/DataBase.hpp"

namespace Common
{
namespace Game
{

class Universe
{
public:
	Universe(Common::DataBase::DataBase & db);

    void add(boost::shared_ptr<Object::IObject> object);
    std::vector<boost::shared_ptr<Object::IObject> > getPlayerObjects(unsigned playerId);
};

}
}
