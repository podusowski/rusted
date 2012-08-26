#include "Cake/Diagnostics/Logger.hpp"
#include "FlightTrajectory.hpp"

using namespace Common::Game::Object;
using namespace Common::Game;

FlightTrajectory::FlightTrajectory() : m_speed(1)
{
}

void FlightTrajectory::fly(Common::Game::Position destination)
{
    TimeValue time = m_time->getCurrentTime();

    auto position = calculatePosition(time);
    m_description.start = position;
    m_description.destination = destination;
    m_description.startTime = time;

    LOG_DEBUG << "New trajectory: from " << position << " to " << destination << ", start time: " << time << ", speed: " << m_speed;
}

void FlightTrajectory::stop()
{
    auto time = m_time->getCurrentTime();
    auto position = calculatePosition(time);

    m_description.start = position;
    m_description.destination = position;

    LOG_DEBUG << "Stoped at: " << position;
}

void FlightTrajectory::setPosition(Common::Game::Position position)
{
    m_description.start = position;
    m_description.destination = position;
}

Common::Game::Position FlightTrajectory::getPosition()
{
    return calculatePosition(m_time->getCurrentTime());
}

void FlightTrajectory::setSpeed(unsigned speed)
{
    m_speed = speed;
}

FlightTrajectory::Description FlightTrajectory::getDescription()
{
    return m_description;
}

void FlightTrajectory::applyDescription(FlightTrajectory::Description description)
{
    m_description = description;
}

Position FlightTrajectory::calculatePosition(TimeValue time)
{
    if (m_description.destination == m_description.start)
        return m_description.start;

    unsigned distance = Position::distance(m_description.destination, m_description.start);
    unsigned totalTripTime = distance / m_speed;
    TimeValue timeTakenSoFar = time - m_description.startTime;
    float secondsTakenSoFar = timeTakenSoFar.getSeconds() + (timeTakenSoFar.getMiliseconds() / 1000.0);

    if (timeTakenSoFar == TimeValue(0, 0))
        return m_description.start;

    float progress = float(secondsTakenSoFar) / float(totalTripTime);

    if (progress >= 1.0)
    {
        m_description.start = m_description.destination;

        LOG_DEBUG << "Destination " << m_description.destination << " reached";

        return m_description.destination;
    }
    else
    {
        Position trajectoryVector = m_description.destination - m_description.start;
        return m_description.start + (trajectoryVector * progress);
    }
}

