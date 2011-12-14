#pragma once

#include <vector>
#include <boost/shared_ptr.hpp>

#include "Object/ObjectFactory.hpp"
#include "Game/Object/IObject.hpp"
#include "DataBase/DataBase.hpp"

namespace Common
{
namespace Game
{

namespace Detail
{
    class ObjectHolder
    {
        unsigned id;
        boost::shared_ptr<Object::IObject> object;
    };
}

class Universe
{
public:
    void add(unsigned id, boost::shared_ptr<Object::IObject> object);
    //std::vector<boost::shared_ptr<Object::IObject> > getPlayerObjects(unsigned playerId);

    template<class ObjectType> ObjectType & getById(unsigned id)
    {
    }

private:
    std::vector<boost::shared_ptr<Object::IObject> > m_objects;
};

}
}
