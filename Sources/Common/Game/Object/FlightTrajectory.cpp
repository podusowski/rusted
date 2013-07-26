#include "Cake/Diagnostics/Logger.hpp"
#include "FlightTrajectory.hpp"
#include "Common/Math/KinematicParticle.hpp"

using namespace Common::Game::Object;
using namespace Common::Game;

FlightTrajectory::FlightTrajectory() :
    m_maxSpeed(1),
    m_acceleration(100),
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
    auto currentSpeed = getCurrentSpeed();

    m_description.controlPoints.clear();

    auto p0 = position;
    auto p1 = calculateOrientationControlPoint(position);
    auto p2 = destination;

    m_description.controlPoints.push_back(p0);
    m_description.controlPoints.push_back(p1);
    m_description.controlPoints.push_back(p2);

    m_description.startTime = time;
    m_description.initialSpeed = currentSpeed;

    configureBezier();

    LOG_DEBUG << "New trajectory: from " << position
              << " to " << destination
              << ", start time: " << time
              << ", max speed: " << m_maxSpeed
              << ", current speed: " << currentSpeed;
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

Common::Game::Position FlightTrajectory::getDestination()
{
    if (m_spline->empty())
    {
        return getPosition();
    }
    else
    {
        return m_description.controlPoints[m_description.controlPoints.size() - 1];
    }
}

Common::Math::Quaternion FlightTrajectory::getOrientation()
{
    auto progress = calculateProgress(m_time->getCurrentTime());
    calculateCachedPositionAndOrientation(progress);
    revalidateProgress(progress);

    return m_cachedOrientation;
}

void FlightTrajectory::setOrientation(const Common::Math::Quaternion & orientation)
{
    m_cachedOrientation = orientation;
}

void FlightTrajectory::setMaxSpeed(unsigned speed)
{
    m_maxSpeed = speed;
}

unsigned FlightTrajectory::getMaxSpeed()
{
    return m_maxSpeed;
}

unsigned FlightTrajectory::getCurrentSpeed()
{
    if (m_spline->empty())
    {
        return 0;
    }

    unsigned distance = m_spline->getLength();
    TimeValue timeTakenSoFar = m_time->getCurrentTime() - m_description.startTime;
    Common::Math::KinematicParticle kinematicParticle(m_maxSpeed, m_acceleration, distance, m_description.initialSpeed);

    if (kinematicParticle.isInRange(timeTakenSoFar))
    {
        return kinematicParticle.calculateSpeed(timeTakenSoFar);
    }
    else
    {
        return 0;
    }
}

void FlightTrajectory::setAcceleration(unsigned acceleration)
{
    m_acceleration = acceleration;
}

bool FlightTrajectory::isMoving()
{
    auto progress = calculateProgress(m_time->getCurrentTime());
    calculateCachedPositionAndOrientation(progress);
    revalidateProgress(progress);

    return !m_spline->empty();
}

FlightTrajectory::Description FlightTrajectory::getDescription()
{
    return m_description;
}

void FlightTrajectory::applyDescription(FlightTrajectory::Description description)
{
    LOG_DEBUG << "Applying precalculated trajectory: " << description
              << ", now: " << m_time->getCurrentTime()
              << ", current position: " << getPosition();

    m_description = compensateLag(description);
    configureBezier();
}

Position FlightTrajectory::calculateOrientationControlPoint(const Position & position) const
{
    auto direction = m_cachedOrientation * Common::Math::Point3(0, 0, 1);
    direction.normalize();
    return position + (direction * 1000);
}

Position FlightTrajectory::calculatePosition(float progress)
{
    if (m_spline->empty())
    {
        return m_cachedPosition;
    }
    else if (progress >= 1.0)
    {
        return m_spline->value(1.0);
    }
    else
    {
        return m_spline->value(progress);
    }
}

Common::Math::Quaternion FlightTrajectory::calculateOrientation(float progress)
{
    Common::Math::Point3 derivative;

    if (m_spline->empty())
    {
        return m_cachedOrientation;
    }
    else if (progress >= 1.0)
    {
        derivative = m_spline->derivative(1.0);
    }
    else
    {
        derivative = m_spline->derivative(progress);
    }

    return Common::Math::Quaternion(std::make_tuple(derivative.getX(), derivative.getY(), derivative.getZ()));
}

float FlightTrajectory::calculateProgress(TimeValue time)
{
    if (m_spline->empty())
    {
        return 0.0;
    }

    unsigned distance = m_spline->getLength();
    TimeValue timeTakenSoFar = time - m_description.startTime;
    Common::Math::KinematicParticle kinematicParticle(m_maxSpeed, m_acceleration, distance, m_description.initialSpeed);

    if (kinematicParticle.isInRange(timeTakenSoFar))
    {
        return kinematicParticle.calculateDistance(timeTakenSoFar) / distance;
    }
    else
    {
        return 1.1; // invalid value
    }
}

void FlightTrajectory::configureBezier()
{
    m_spline->reset();

    LOG_DEBUG << "Configuring bezier:";

    for (const auto & p : m_description.controlPoints)
    {
        LOG_DEBUG << "  " << p;
        m_spline->addControlPoint(p);
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

FlightTrajectory::Description FlightTrajectory::compensateLag(const FlightTrajectory::Description & description)
{
    const unsigned positionMaxCompensationThreshold = 2000;
    const unsigned positionMinCompensationThreshold = 0;
    const TimeValue timeCompensationThreshold(1, 0);

    FlightTrajectory::Description ret = description;

    LOG_DEBUG << "Performing lag control on new flight description";

    if (description.controlPoints.size() >= 3)
    {
        auto newStartingPosition = *description.controlPoints.begin();
        auto currentPosition = getPosition();
        auto offset = newStartingPosition - currentPosition;
        bool anyOfCompensationsPerformed = false;

        // if thresholds are met, add control point with current position at the beginning
        // of the curve
        if (offset.length() < positionMaxCompensationThreshold && offset.length() > positionMinCompensationThreshold)
        {
            // starting point
            LOG_DEBUG << "Position offset: " << offset << " (length: " << offset.length() << ") smaller than threshold, compensating";
            LOG_DEBUG << "  " << description.controlPoints[0] << " replacing with " << currentPosition << " at first control point";
            ret.controlPoints[0] = currentPosition;

            // second control point (directional control point)
            auto orientationControlPoint = calculateOrientationControlPoint(currentPosition);
            LOG_DEBUG << "  " << description.controlPoints[1] << " replacing with " << currentPosition << " at second (orientation) control point";
            ret.controlPoints[1] = orientationControlPoint;

            anyOfCompensationsPerformed = true;
        }

        auto now = m_time->getCurrentTime();
        auto timeOffset = now - description.startTime;
        if (timeOffset < timeCompensationThreshold)
        {
            LOG_DEBUG << "Time offset: " << timeOffset << " smaller than threshold(" << timeCompensationThreshold << "), compensating";
            ret.startTime = now;

            anyOfCompensationsPerformed = true;
        }

        if (anyOfCompensationsPerformed)
        {
            unsigned currentSpeed = getCurrentSpeed();
            LOG_DEBUG << "Compensating current speed, network: " << ret.initialSpeed << ", new: " << currentSpeed;
            ret.initialSpeed = currentSpeed;
        }
    }

    return ret;
}

