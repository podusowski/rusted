#pragma once

#include "Common/Game/TimeValue.hpp"

namespace Common
{
namespace Math
{

/**
 * KinematicParticle represents particle movement in 1 dimension. It takes
 * under consideration maximum speed, acceleration and deceleration.
 *
 * To take into account deceleration, this object needs to know what is
 * the target distance for the particle.
 */
class KinematicParticle
{
public:
    KinematicParticle(float maxSpeed, float acceleration, float targetDistance);
    float calculateDistance(Common::Game::TimeValue deltaTime);

private:
    float m_maxSpeed;
    float m_acceleration;
    float m_targetDistance;
};

}
}

