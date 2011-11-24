#pragma once

#include <boost/shared_ptr.hpp>
#include "Object/IObject.hpp"

namespace Common
{
namespace Game
{
namespace Object
{

class ObjectFactory
{
public:
	boost::shared_ptr<IObject> createObject(DataBase::DataBaseNode & data);
};

}
}
}
