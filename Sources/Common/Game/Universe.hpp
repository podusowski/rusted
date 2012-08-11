#pragma once

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

#include "Cake/Diagnostics/Logger.hpp"

#include "Game/Object/ObjectBase.hpp"
#include "Game/Object/OwnedObjectBase.hpp"
#include "Game/Utilities/IdGenerator.hpp"

namespace Common
{
namespace Game
{

class Universe
{
public:
    typedef std::vector<boost::shared_ptr<Object::ObjectBase> > Objects;
    typedef boost::function<void(Common::Game::Object::ObjectBase &)> ObjectAddedCallback;

    void add(boost::shared_ptr<Object::ObjectBase> object);

    unsigned reserveId();

    bool has(unsigned id);

    template<class ObjectType> ObjectType & getById(unsigned id)
    {
        std::map<unsigned, boost::shared_ptr<Object::ObjectBase> >::iterator it = m_objects.find(id);

        if (it != m_objects.end())
        {
            ObjectType * object = dynamic_cast<ObjectType*>(it->second.get());
            if (object)
            {
                return *object;
            }
            else
            {
                std::stringstream ss;
                ss << "object id: " << id << " is of type " 
                   << TYPENAME(*it->second) << " while " << TYPENAME(ObjectType) << " requested";
                throw std::runtime_error(ss.str());
            }
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

    Objects getAll()
    {
        Objects ret;

        for (std::map<unsigned, boost::shared_ptr<Object::ObjectBase> >::iterator it = m_objects.begin();
             it != m_objects.end(); it++)
        {
            ret.push_back(it->second);
        }

        return ret;

    }

    void setObjectAddedCallback(ObjectAddedCallback);

private:
    std::map<unsigned, boost::shared_ptr<Object::ObjectBase> > m_objects;
    ObjectAddedCallback m_objectAddedCallback;
    Common::Game::Utilities::IdGenerator m_idGenerator;
};

}
}
