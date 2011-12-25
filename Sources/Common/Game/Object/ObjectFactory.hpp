#pragma once

#include <boost/shared_ptr.hpp>

#include "IObject.hpp"
#include "DataBase/DataBaseNode.hpp"

namespace Common
{
namespace Game
{
namespace Object
{

class ObjectFactory
{
public:
	boost::shared_ptr<IObject> create(DataBase::DataBaseNode & data);
};

}
}
}
