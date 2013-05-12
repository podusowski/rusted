#include <cmath>
#include <stdexcept>

#include "KinematicParticle.hpp"

using namespace Common::Math;

KinematicParticle::KinematicParticle(float maxSpeed, float acceleration, float targetDistance) :
    m_maxSpeed(maxSpeed),
    m_acceleration(acceleration),
    m_targetDistance(targetDistance),

    // precalculated stuff
    m_Tmax(m_targetDistance / m_maxSpeed + (m_maxSpeed / m_acceleration)),
    m_t1(m_maxSpeed / m_acceleration),
    m_t2(m_Tmax - m_t1)
{
}

float KinematicParticle::calculateDistance(Common::Game::TimeValue deltaTime) const
{
    // see unit tests for more description on the algorithm

    float t = deltaTime.getSeconds() + (deltaTime.getMiliseconds() / 1000.0);

    float S = 0;

    if (t < m_t1)
    {
        S = m_acceleration * std::pow(t, 2) / 2.0;
    }
    else if (t >= m_t1 && t < m_t2)
    {
        S = m_t1 * m_maxSpeed / 2.0;
        S += (t - m_t1) * m_maxSpeed;
    }
    else if (t > m_t2 && t <= m_Tmax)
    {
        S = m_maxSpeed * m_t1;
        S += m_maxSpeed * (m_t2 - m_t1);
        S -= m_acceleration * std::pow(m_Tmax - t, 2) / 2.0;
    }
    else
    {
        throw std::out_of_range("deltaTime passes Tmax");
    }

    return S;
}

float KinematicParticle::calculateSpeed(Common::Game::TimeValue deltaTime) const
{
    return 0;
}

bool KinematicParticle::isInRange(Common::Game::TimeValue deltaTime) const
{
    float t = deltaTime.getSeconds() + (deltaTime.getMiliseconds() / 1000.0);
    return t <= m_Tmax;
}

