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
    typedef std::vector<boost::shared_ptr<Object::ObjectBase> > Objects;

    void add(boost::shared_ptr<Object::ObjectBase> object);

    template<class ObjectType> ObjectType & getById(unsigned id)
    {
        std::map<unsigned, boost::shared_ptr<Object::ObjectBase> >::iterator it = m_objects.find(id);

        if (it != m_objects.end())
        {
            return dynamic_cast<ObjectType&>(*it->second);
        }
        else
        {
            throw std::out_of_range("no such object in universe");
        }
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

    template<class ObjectType> std::vector<boost::shared_ptr<Object::ObjectBase> > get()
    {
        std::vector<boost::shared_ptr<Object::ObjectBase> > ret;

        for (std::map<unsigned, boost::shared_ptr<Object::ObjectBase> >::iterator it = m_objects.begin();
             it != m_objects.end(); it++)
        {
            if (typeid(*it->second) == typeid(ObjectType))
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
