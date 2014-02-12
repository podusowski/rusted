#pragma once

#include <vector>
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
    typedef std::vector<std::shared_ptr<Object::ObjectBase> > Objects;
    typedef boost::function<void(Common::Game::Object::ObjectBase &)> ObjectAddedCallback;

    void add(std::shared_ptr<Object::ObjectBase> object);
    bool has(unsigned id);
    bool isOwnedBy(unsigned objectId, unsigned ownerId);

    // TODO: deprecated
    template<class ObjectType> ObjectType & getById(unsigned id)
    {
        std::map<unsigned, std::shared_ptr<Object::ObjectBase> >::iterator it = m_objects.find(id);

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

    // TODO: deprecated
    template<class ObjectType> std::vector<std::shared_ptr<Object::ObjectBase> > getByOwnerId(unsigned id)
    {
        std::vector<std::shared_ptr<Object::ObjectBase> > ret;

        for (std::map<unsigned, std::shared_ptr<Object::ObjectBase> >::iterator it = m_objects.begin();
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

    // TODO: deprecated
    template<class ObjectType> std::vector<std::shared_ptr<Object::ObjectBase> > get()
    {
        std::vector<std::shared_ptr<Object::ObjectBase> > ret;

        for (std::map<unsigned, std::shared_ptr<Object::ObjectBase> >::iterator it = m_objects.begin();
             it != m_objects.end(); it++)
        {
            if (it->second->is<ObjectType>())
            {
                ret.push_back(it->second);
            }
        }

        return ret;
    }

    // TODO: deprecated
    Objects getAll()
    {
        Objects ret;

        for (std::map<unsigned, std::shared_ptr<Object::ObjectBase> >::iterator it = m_objects.begin();
             it != m_objects.end(); it++)
        {
            ret.push_back(it->second);
        }

        return ret;

    }

    void addObjectAddedCallback(ObjectAddedCallback);

    void objectsInProximity(Position center, int radius, std::function<void(Object::ObjectBase &)>);
    void objectsOwnedByPlayer(int playerId, std::function<void(Object::ObjectBase &)>);

private:
    std::map<unsigned, std::shared_ptr<Object::ObjectBase> > m_objects;
    std::vector<ObjectAddedCallback> m_objectAddedCallbacks;
    Common::Game::Utilities::IdGenerator m_idGenerator;
};

}
}
