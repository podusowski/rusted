#include <stdexcept>

#include "Cake/Diagnostics/Logger.hpp"

#include "Universe.hpp"

using namespace Common::Game;

void Universe::add(boost::shared_ptr<Object::ObjectBase> object)
{
    LOG_DEBUG << "Adding " << TYPENAME(*object) << " with id: " << object->getId();

    auto ret = m_objects.insert(std::make_pair(object->getId(), object));

    if (not ret.second)
    {
        throw std::runtime_error("element already exists");
    }

    if (m_objectAddedCallback)
        m_objectAddedCallback(*object);
}

void Universe::setObjectAddedCallback(ObjectAddedCallback callback)
{
    LOG_DEBUG << "Setting ObjectAddedCallback";

    m_objectAddedCallback = callback;
}
