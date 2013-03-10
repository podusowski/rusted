#include "Cake/Diagnostics/Logger.hpp"
#include "FlightTrajectory.hpp"

using namespace Common::Game::Object;
using namespace Common::Game;

FlightTrajectory::FlightTrajectory() : 
    m_speed(1),
    m_lastOrientation(Common::Math::Quaternion(0, std::make_tuple(0, 0, 1)))
{
}

FlightTrajectory::~FlightTrajectory()
{
}

void FlightTrajectory::fly(Common::Game::Position destination)
{
    TimeValue time = m_time->getCurrentTime();

    auto position = calculatePosition(time);
    m_description.start = position;
    m_description.destination = destination;
    m_description.startTime = time;

    configureBezier();

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

Common::Math::Quaternion FlightTrajectory::getOrientation()
{
    if (m_description.destination != m_description.start)
    {
        auto time = m_time->getCurrentTime();
        float progress = calculateProgress(time);
        auto derivative = m_bezier.derivative(progress);

        Common::Game::Position unit(0, 1, 0);
        m_lastOrientation = unit.getRotationTo(derivative);
        m_lastOrientation.normalize();
    }

    return m_lastOrientation;
}

void FlightTrajectory::setSpeed(unsigned speed)
{
    m_speed = speed;
}

unsigned FlightTrajectory::getSpeed()
{
    return m_speed;
}

FlightTrajectory::Description FlightTrajectory::getDescription()
{
    return m_description;
}

void FlightTrajectory::applyDescription(FlightTrajectory::Description description)
{
    m_description = description;
    configureBezier();
}

Position FlightTrajectory::calculatePosition(TimeValue time)
{
    if (m_bezier.empty())
    {
        return m_description.start;
    }

    float progress = calculateProgress(time);

    if (progress >= 1.0)
    {
        m_description.start = m_description.destination;
        m_bezier.reset();

        LOG_DEBUG << "Destination " << m_description.destination << " reached";

        return m_description.destination;
    }
    else
    {
        return m_bezier(progress);
    }
}

float FlightTrajectory::calculateProgress(TimeValue time)
{
    if (m_bezier.empty())
    {
        return 0.0;
    }

    unsigned distance = m_bezier.getLength();
    unsigned totalTripTime = distance / m_speed;
    TimeValue timeTakenSoFar = time - m_description.startTime;
    float secondsTakenSoFar = timeTakenSoFar.getSeconds() + (timeTakenSoFar.getMiliseconds() / 1000.0);

    return float(secondsTakenSoFar) / float(totalTripTime);
}

void FlightTrajectory::configureBezier()
{
    m_bezier.reset();
    m_bezier.addControlPoint(m_description.start);
    m_bezier.addControlPoint(m_description.destination);
}

