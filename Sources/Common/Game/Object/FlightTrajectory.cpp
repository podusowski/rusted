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
    float progress = calculateProgress(time);
    auto position = calculatePosition(progress);

    m_description.controlPoints.clear();

    if (m_bezier.empty())
    {
        m_description.controlPoints.push_back(position);
        m_description.controlPoints.push_back(destination);
    }
    else
    {
        auto tangent = m_bezier.derivative(progress);
        tangent.normalize();

        auto p0 = position;
        auto p1 = position + (tangent * 1000);
        auto p2 = destination;

        m_description.controlPoints.push_back(p0);
        m_description.controlPoints.push_back(p1);
        m_description.controlPoints.push_back(p2);
    }

    m_description.startTime = time;

    configureBezier();

    LOG_DEBUG << "New trajectory: from " << position << " to " << destination << ", start time: " << time << ", speed: " << m_speed;
}

void FlightTrajectory::stop()
{
    auto time = m_time->getCurrentTime();
    auto progress = calculateProgress(time);
    auto position = calculatePosition(progress);

    m_cachedPosition = position;

    m_description.controlPoints.clear();
    configureBezier();

    LOG_DEBUG << "Stoped at: " << position;
}

void FlightTrajectory::setPosition(Common::Game::Position position)
{
    m_cachedPosition = position;

    m_description.controlPoints.clear();
    configureBezier();
}

Common::Game::Position FlightTrajectory::getPosition()
{
    return calculatePosition(calculateProgress(m_time->getCurrentTime()));
}

Common::Game::Position FlightTrajectory::getCourseMarkerPosition()
{
    float progress = calculateProgress(m_time->getCurrentTime()) * 20;
    progress -= floor(progress);
    return calculatePosition(progress);
}

Common::Math::Quaternion FlightTrajectory::getCourseMarkerOrientation()
{
    return Common::Math::Quaternion();
}

Common::Math::Quaternion FlightTrajectory::getOrientation()
{
    // revalidate bezier
    auto time = m_time->getCurrentTime();
    float progress = calculateProgress(time);
    calculatePosition(progress);

    if (!m_bezier.empty())
    {
        auto derivative = m_bezier.derivative(progress);
        m_lastOrientation = Common::Math::Quaternion(std::make_tuple(derivative.getX(), derivative.getY(), derivative.getZ()));
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

Position FlightTrajectory::calculatePosition(float progress)
{
    if (m_bezier.empty())
    {
        return m_cachedPosition;
    }

    if (progress >= 1.0)
    {
        m_cachedPosition = m_bezier(1.0);

        auto derivative = m_bezier.derivative(progress);
        m_lastOrientation = Common::Math::Quaternion(std::make_tuple(derivative.getX(), derivative.getY(), derivative.getZ()));

        m_description.controlPoints.clear();
        configureBezier();

        LOG_DEBUG << "Destination " << m_cachedPosition << " reached";

        return m_cachedPosition;
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
    
    LOG_DEBUG << "Configuring bezier:";

    for (const auto & p : m_description.controlPoints)
    {
        LOG_DEBUG << "  " << p;
        m_bezier.addControlPoint(p);
    }
}

