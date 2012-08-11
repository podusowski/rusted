#include <stdexcept>

#include "Cake/Diagnostics/Logger.hpp"

#include "Universe.hpp"

using namespace Common::Game;

void Universe::add(boost::shared_ptr<Object::ObjectBase> object)
{
    LOG_DEBUG << "Adding " << TYPENAME(*object) << " with id: " << object->getId();

    m_idGenerator.reserve(object->getId());

    auto ret = m_objects.insert(std::make_pair(object->getId(), object));

    if (not ret.second)
    {
        throw std::runtime_error("element already exists");
    }

    if (m_objectAddedCallback)
        m_objectAddedCallback(*object);
}

unsigned Universe::reserveId()
{
    return m_idGenerator.generate();
}

bool Universe::has(unsigned id)
{
    auto it = m_objects.find(id);
    return it != m_objects.end();
}

void Universe::setObjectAddedCallback(ObjectAddedCallback callback)
{
    LOG_DEBUG << "Setting ObjectAddedCallback";

    m_objectAddedCallback = callback;
}
