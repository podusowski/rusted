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

unsigned Asteroid::getHelium()
{
    return m_helium;
}

void Asteroid::setHelium(unsigned helium)
{
    m_helium = helium;
}

unsigned Asteroid::getCarbon()
{
    return m_carbon;
}

void Asteroid::setCarbon(unsigned carbon)
{
    m_carbon = carbon;
}
