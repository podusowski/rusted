#include <stdexcept>

#include "Cake/Diagnostics/Logger.hpp"

#include "Universe.hpp"

using namespace Common::Game;

void Universe::add(std::shared_ptr<Object::ObjectBase> object)
{
    LOG_DEBUG << "Adding " << TYPENAME(*object) << " with id: " << object->getId();

    if (object->getId() == 0)
    {
        unsigned id = m_idGenerator.generate();

        LOG_DEBUG << "Object has invalid id, assigning new one: " << id;

        object->setId(id);
    }
    else
    {
        m_idGenerator.reserve(object->getId());
    }

    auto ret = m_objects.insert(std::make_pair(object->getId(), object));

    if (not ret.second)
    {
        throw std::runtime_error("element already exists");
    }

    for (auto callback: m_objectAddedCallbacks)
        callback(*object);
}

bool Universe::has(unsigned id)
{
    auto it = m_objects.find(id);
    return it != m_objects.end();
}

bool Universe::isOwnedBy(unsigned objectId, unsigned ownerId)
{
    auto it = m_objects.find(objectId);

    if (it == m_objects.end())
    {
        return false;
    }

    auto * ownedObject = dynamic_cast<Common::Game::Object::OwnedObjectBase*>(it->second.get());

    if (!ownedObject)
    {
        return false;
    }

    if (ownedObject->getOwnerId() != ownerId)
    {
        return false;
    }

    return true;
}

void Universe::addObjectAddedCallback(ObjectAddedCallback callback)
{
    // TODO: use boost::signals2 for thread safety
    m_objectAddedCallbacks.push_back(callback);
}

void Universe::objectsInProximity(Position center, int radius, std::function<void(Object::ObjectBase &)> callback)
{
    for (auto pair: m_objects)
    {
        auto & object = pair.second;
        if (Position::distance(center, object->getPosition()) < radius)
        {
            callback(*object);
        }
    }
}

void Universe::objectsOwnedByPlayer(int playerId, std::function<void(Object::ObjectBase &)> callback)
{
    for (const auto pair: m_objects)
    {
        const auto & object = pair.second;
        if (object->is<Object::OwnedObjectBase>() && dynamic_cast<Object::OwnedObjectBase&>(*object).getOwnerId() == playerId)
        {
            callback(*object);
        }
    }
}

