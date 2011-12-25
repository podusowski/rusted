#pragma once

#include <vector>
#include <boost/shared_ptr.hpp>

#include "Object/ObjectFactory.hpp"
#include "Game/Object/ObjectBase.hpp"
#include "DataBase/DataBase.hpp"

namespace Common
{
namespace Game
{

class Universe
{
public:
    void add(boost::shared_ptr<Object::ObjectBase> object);

    template<class ObjectType> ObjectType & getById(unsigned id)
    {
        return dynamic_cast<ObjectType&>(*m_objects[id]);
    }

private:
    std::map<unsigned, boost::shared_ptr<Object::ObjectBase> > m_objects;
};

}
}
