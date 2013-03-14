#include "Cake/Diagnostics/Logger.hpp"
#include "FlightTrajectory.hpp"

using namespace Common::Game::Object;
using namespace Common::Game;

FlightTrajectory::FlightTrajectory() : 
    m_speed(1),
    m_cachedOrientation(Common::Math::Quaternion(0, std::make_tuple(0, 0, 1)))
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

    // TODO: is this be faster than derivative calc?
    auto direction = m_cachedOrientation * Common::Math::Point3(0, 0, 1);
    direction.normalize();

    auto p0 = position;
    auto p1 = position + (direction * 1000);
    auto p2 = destination;

    m_description.controlPoints.push_back(p0);
    m_description.controlPoints.push_back(p1);
    m_description.controlPoints.push_back(p2);

    m_description.startTime = time;

    configureBezier();

    LOG_DEBUG << "New trajectory: from " << position << " to " << destination << ", start time: " << time << ", speed: " << m_speed;
}

void FlightTrajectory::stop()
{
    auto time = m_time->getCurrentTime();
    auto progress = calculateProgress(time);

    calculateCachedPositionAndOrientation(progress);

    m_description.controlPoints.clear();
    configureBezier();

    LOG_DEBUG << "Stoped at: " << m_cachedPosition;
}

void FlightTrajectory::setPosition(Common::Game::Position position)
{
    m_cachedPosition = position;

    m_description.controlPoints.clear();
    configureBezier();
}

Common::Game::Position FlightTrajectory::getPosition()
{
    auto progress = calculateProgress(m_time->getCurrentTime());
    calculateCachedPositionAndOrientation(progress);
    revalidateProgress(progress);

    return m_cachedPosition;
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
    auto progress = calculateProgress(m_time->getCurrentTime());
    calculateCachedPositionAndOrientation(progress);
    revalidateProgress(progress);

    return m_cachedOrientation;
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
    else if (progress >= 1.0)
    {
        return m_bezier(1.0);
    }
    else
    {
        return m_bezier(progress);
    }
}

Common::Math::Quaternion FlightTrajectory::calculateOrientation(float progress)
{
    Common::Math::Point3 derivative;

    if (m_bezier.empty())
    {
        return m_cachedOrientation;
    }
    else if (progress >= 1.0)
    {
        derivative = m_bezier.derivative(1.0);
    }
    else
    {
        derivative = m_bezier.derivative(progress);
    }

    return Common::Math::Quaternion(std::make_tuple(derivative.getX(), derivative.getY(), derivative.getZ()));
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

void FlightTrajectory::revalidateProgress(float progress)
{
    if (progress > 1.0)
    {
        m_description.controlPoints.clear();
        configureBezier();
    }
}

void FlightTrajectory::calculateCachedPositionAndOrientation(float progress)
{
    m_cachedPosition = calculatePosition(progress);
    m_cachedOrientation = calculateOrientation(progress);
}

