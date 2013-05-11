#include <cmath>
#include <stdexcept>

#include "KinematicParticle.hpp"

using namespace Common::Math;

KinematicParticle::KinematicParticle(float maxSpeed, float acceleration, float targetDistance) :
    m_maxSpeed(maxSpeed),
    m_acceleration(acceleration),
    m_targetDistance(targetDistance)
{
}

float KinematicParticle::calculateDistance(Common::Game::TimeValue deltaTime)
{
    // see unit tests for more description on the algorithm

    // TODO: precalculate some of those
    float t = deltaTime.getSeconds() + (deltaTime.getMiliseconds() / 1000.0);
    float Tmax = m_targetDistance / m_maxSpeed + (m_maxSpeed / m_acceleration);
    float t1 = m_maxSpeed / m_acceleration;
    float t2 = Tmax - t1;

    float S = 0;

    if (t < t1)
    {
        S = m_acceleration * std::pow(t, 2) / 2.0;
    }
    else if (t >= t1 && t < t2)
    {
        S = t1 * m_maxSpeed / 2.0;
        S += (t - t1) * m_maxSpeed;
    }
    else if (t > t2 && t <= Tmax)
    {
        S = m_maxSpeed * t1;
        S += m_maxSpeed * (t2 - t1);
        S -= m_acceleration * std::pow(Tmax - t, 2) / 2.0;
    }
    else
    {
        throw std::out_of_range("deltaTime passes Tmax");
    }

    return S;
}

