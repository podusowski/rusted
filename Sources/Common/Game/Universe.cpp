#include <stdexcept>

#include "Cake/Diagnostics/Logger.hpp"

#include "Universe.hpp"

using namespace Common::Game;

void Universe::add(boost::shared_ptr<Object::ObjectBase> object)
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

void Universe::addObjectAddedCallback(ObjectAddedCallback callback)
{
    LOG_DEBUG << "Adding ObjectAddedCallback: " << callback;

    m_objectAddedCallbacks.push_back(callback);
}
