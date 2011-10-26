#include "Common/Game/StaticObject.hpp"

using namespace Common::Game;

StaticObject::StaticObject(int id, StaticObject::Position position) : 
    m_id(id), 
    m_position(position)
{
}

int StaticObject::getId()
{
    return m_id;
}

StaticObject::Position StaticObject::getPosition()
{
    return m_position;
}

