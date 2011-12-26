#pragma once

#include <vector>
#include <boost/shared_ptr.hpp>

#include "Object/ObjectFactory.hpp"
#include "Game/Object/ObjectBase.hpp"
#include "Game/Object/OwnedObjectBase.hpp"
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

    template<class ObjectType> std::vector<boost::shared_ptr<Object::ObjectBase> > getByOwnerId(unsigned id)
    {
        std::vector<boost::shared_ptr<Object::ObjectBase> > ret;

        for (std::map<unsigned, boost::shared_ptr<Object::ObjectBase> >::iterator it = m_objects.begin();
             it != m_objects.end(); it++)
        {
            if (typeid(*it->second) == typeid(ObjectType) 
                && dynamic_cast<Object::OwnedObjectBase&>(*it->second).getOwnerId() == id)
            {
                ret.push_back(it->second);
            }
        }

        return ret;
    }

private:
    std::map<unsigned, boost::shared_ptr<Object::ObjectBase> > m_objects;
};

}
}
