#include "Asteroid.hpp"

using namespace Common::Game::Object;
using namespace Common::Game;

void Asteroid::setPosition(const Position & position)
{
    m_position = position;
}

Position Asteroid::getPosition()
{
    return m_position;
}
