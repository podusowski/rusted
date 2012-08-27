#include "Cake/Diagnostics/Logger.hpp"

#include "Ship.hpp"

using namespace Common::Game::Object;
using namespace Common::Game;

Ship::Ship() : m_speed(1)
{
}

Position Ship::getPosition()
{
    return m_trajectory->getPosition();
}

void Ship::setPosition(const Position & position)
{
    m_trajectory->setPosition(position);
}

void Ship::setCourse(Position position)
{
    if (getIntegrity() == 0)
    {
        LOG_DEBUG << "Ship is destroyed and can't go anywhere";
        return;
    }

    m_trajectory->fly(position);
}

void Ship::setSpeed(unsigned speed)
{
    m_speed = speed;
}

unsigned Ship::getSpeed()
{
    return m_speed;
}

void Ship::setIntegrity(unsigned integrity)
{
    ObjectBase::setIntegrity(integrity);

    if (getIntegrity() == 0)
    {
        m_trajectory->stop();
    }
}

IFlightTrajectory::Description Ship::getTrajectoryDescription()
{
    return m_trajectory->getDescription();
}

void Ship::applyTrajectoryDescription(IFlightTrajectory::Description description)
{
    m_trajectory->applyDescription(description);
}

