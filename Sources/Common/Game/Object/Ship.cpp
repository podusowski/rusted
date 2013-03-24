#include "Cake/Diagnostics/Logger.hpp"

#include "Ship.hpp"

using namespace Common::Game::Object;
using namespace Common::Game;

Ship::Ship() : m_class(0)
{
}

Ship::~Ship()
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

bool Ship::isMoving()
{
    return m_trajectory->isMoving();
}

Position Ship::getCourseMarkerPosition()
{
    return m_trajectory->getCourseMarkerPosition();
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
    m_trajectory->setSpeed(speed);
}

unsigned Ship::getSpeed()
{
    return m_trajectory->getSpeed();
}

void Ship::setIntegrity(unsigned integrity)
{
    ObjectBase::setIntegrity(integrity);

    if (getIntegrity() == 0)
    {
        m_trajectory->stop();
    }
}

Common::Math::Quaternion Ship::getOrientation()
{
    return m_trajectory->getOrientation();
}

void Ship::setClass(unsigned shipClass)
{
    m_class = shipClass;
}

unsigned Ship::getClass()
{
    return m_class;
}

IFlightTrajectory::Description Ship::getTrajectoryDescription()
{
    return m_trajectory->getDescription();
}

void Ship::applyTrajectoryDescription(IFlightTrajectory::Description description)
{
    m_trajectory->applyDescription(description);
}

