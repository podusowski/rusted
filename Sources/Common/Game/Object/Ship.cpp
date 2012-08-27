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

Position Ship::calculatePosition(TimeValue time)
{
    if (m_course.destination == m_position)
        return m_position;

    unsigned distance = Position::distance(m_course.destination, m_position);
    unsigned totalTripTime = distance / m_speed;
    TimeValue timeTakenSoFar = time - m_course.startTime;
    float secondsTakenSoFar = timeTakenSoFar.getSeconds() + (timeTakenSoFar.getMiliseconds() / 1000.0);


    if (timeTakenSoFar == TimeValue(0, 0))
        return m_position;

    float tripProgress = float(secondsTakenSoFar) / float(totalTripTime);

    if (tripProgress >= 1.0)
    {
        m_position = m_course.destination;

        LOG_DEBUG << "Ship arrived at " << m_position;

        return m_position;
    }
    else
    {
        Position tripVector = m_course.destination - m_position;
        return m_position + (tripVector * tripProgress);
    }
}

