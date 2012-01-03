#include "StaticObject.hpp"

using namespace Common::Game::Object;
using namespace Common::Game;

void StaticObject::setPosition(const Position & position)
{
    m_position = position;
}

Position StaticObject::getPosition()
{
    return m_position;
}
