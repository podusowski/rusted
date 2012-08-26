#include "FlightTrajectory.hpp"

using namespace Common::Game::Object;

FlightTrajectory::FlightTrajectory() : m_speed(1)
{
}

void FlightTrajectory::fly(Common::Game::Position destination)
{
}

void FlightTrajectory::stop()
{
}

void FlightTrajectory::setPosition(Common::Game::Position position)
{
}

Common::Game::Position FlightTrajectory::getPosition()
{
    return Position();
}

void FlightTrajectory::setSpeed(unsigned speed)
{
}

FlightTrajectory::Description FlightTrajectory::getDescription()
{
    FlightTrajectory::Description description;
    return description;
}

void FlightTrajectory::applyDescription(FlightTrajectory::Description description)
{
}

Common::Game::Position FlightTrajectory::calculatePosition(TimeValue time)
{
#if 0
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
    #endif
}

