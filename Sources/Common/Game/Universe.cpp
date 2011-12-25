#include "Universe.hpp"

using namespace Common::Game;

void Universe::add(boost::shared_ptr<Object::ObjectBase> object)
{
    m_objects.insert(std::make_pair(object->getId(), object));
}

