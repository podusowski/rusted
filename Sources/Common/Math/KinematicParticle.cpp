#include <cmath>
#include <stdexcept>

#include "KinematicParticle.hpp"

using namespace Common::Math;

KinematicParticle::KinematicParticle(float maxSpeed, float acceleration, float targetDistance, float initialSpeed) :
    m_maxSpeed(maxSpeed),
    m_acceleration(acceleration),
    m_targetDistance(targetDistance),
    m_initialSpeed(initialSpeed),

    // precalculated stuff
    m_t1((m_maxSpeed - m_initialSpeed) / m_acceleration),
    m_willReachMaxSpeed(calculateRoadDuringLinearAcceleration(m_acceleration, m_t1) < m_targetDistance / 2),
    m_Tmax(calculateTmax()),
    m_t2(m_Tmax - (m_maxSpeed / m_acceleration))
{
}

float KinematicParticle::calculateDistance(Common::Game::TimeValue deltaTime) const
{
    // see unit tests for more description on the algorithm

    float t = getTimeInSeconds(deltaTime);
    float S = 0;

    if (m_willReachMaxSpeed)
    {
        S += m_initialSpeed * std::min(t, m_t1);
        S += m_acceleration * std::pow(std::min(t, m_t1), 2) / 2.0;

        if (t > m_t1)
        {
            S += (std::min(t, m_t2) - m_t1) * m_maxSpeed;
        }

        if (t > m_t2)
        {
            float realTime = std::min(t, m_Tmax);

            S += (realTime - m_t2) * m_maxSpeed;
            S -= std::pow(realTime - m_t2, 2) * m_acceleration / 2.0;
        }
    }
    else
    {
        float realTime = std::min(t, m_Tmax);
        float halfTime = m_Tmax / 2.0;

        S += calculateRoadDuringLinearAcceleration(m_acceleration, std::min(realTime, halfTime));

        if (realTime > halfTime)
        {
            float eta = m_Tmax - realTime;

            S += calculateRoadDuringLinearAcceleration(
                m_acceleration,
                halfTime - eta,
                eta * m_acceleration);
        }
    }

    return S;
}

float KinematicParticle::calculateSpeed(Common::Game::TimeValue deltaTime) const
{
    float t = getTimeInSeconds(deltaTime);

    if (t > m_t2)
    {
        float realTime = std::min(m_Tmax, t);

        return (m_Tmax - realTime) * m_acceleration;
    }
    else if (t > m_t1)
    {
        return m_maxSpeed;
    }
    else
    {
        return t * m_acceleration;
    }
}

bool KinematicParticle::isInRange(Common::Game::TimeValue deltaTime) const
{
    float t = getTimeInSeconds(deltaTime);
    return t <= m_Tmax;
}

Common::Game::TimeValue KinematicParticle::getEta(Common::Game::TimeValue currentTime) const
{
    return Common::Game::TimeValue::fromSeconds(m_Tmax) - currentTime;
}

float KinematicParticle::getTimeInSeconds(Common::Game::TimeValue time) const
{
    return time.getSeconds() + (time.getMiliseconds() / 1000.0);
}

float KinematicParticle::calculateTmax() const
{
    if (m_willReachMaxSpeed)
    {
        return (
            m_targetDistance +
            (
                std::pow(m_maxSpeed, 2) / float(2 * m_acceleration)
            )
            +
            (
                std::pow(m_maxSpeed - m_initialSpeed, 2) / float(2 * m_acceleration)
            )
        )
        /
        m_maxSpeed;
    }
    else
    {
        return 2 * std::sqrt(m_targetDistance / m_acceleration);
    }
}

float KinematicParticle::calculateRoadDuringLinearAcceleration(float a, float t, float initialSpeed) const
{
    return (initialSpeed * t) + (a * std::pow(t, 2) / 2);
}

