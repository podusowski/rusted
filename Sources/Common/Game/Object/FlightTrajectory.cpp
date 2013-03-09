#include "Cake/Diagnostics/Logger.hpp"
#include "FlightTrajectory.hpp"

using namespace Common::Game::Object;
using namespace Common::Game;

FlightTrajectory::FlightTrajectory() : 
    m_speed(1),
    m_lastOrientation(Common::Math::Quaternion(0, Point3<int>(0, 0, 1)))
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
        int x = m_description.destination.getX() - m_description.start.getX();
        int y = m_description.destination.getY() - m_description.start.getY();

        const float PI = 3.14;

        float angle = 0;

        if (y == 0 || x == 0)
        {
            // TODO
            angle = 0;
        }
        else
        {
            angle = atan(float(abs(x)) / float(abs(y)));
        }

        if (y > 0 && x > 0)
        {
            angle = -angle;
        }
        else if (y > 0 && x < 0)
        {
        }
        else if (y < 0 && x > 0)
        {
            angle = PI + angle;
        }
        else if (y < 0 && x < 0)
        {
            angle = PI - angle;
        }

        m_lastOrientation = Common::Math::Quaternion(angle, Point3<int>(0, 0, 1));
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

