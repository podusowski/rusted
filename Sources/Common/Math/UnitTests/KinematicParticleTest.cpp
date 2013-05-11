#include <gtest/gtest.h>

#include "Common/Math/KinematicParticle.hpp"

using namespace Common::Math;
using Common::Game::TimeValue;

TEST(KinematicParticleTest, FullMovement)
{
    float maxSpeed = 10.0;
    float acceleration = 10.0;
    float targetDistance = 100.0;

    KinematicParticle particle(maxSpeed, acceleration, targetDistance);

    // acceleration phase
    // S = at2/2
    // V = at
    {
        EXPECT_FLOAT_EQ(0.05, particle.calculateDistance(TimeValue(0, 100)));
        EXPECT_FLOAT_EQ(1.25, particle.calculateDistance(TimeValue(0, 500)));
    }

    // steady phase
    // t1 - time when particle stops accelerating and has maximum speed
    // t1 = maxSpeed / acceleration = 10 / 10 = 1
    // S = (maxSpeed * t1) / 2 + // triangle
    //     maxSpeed * (t - t1)   // rect
    {
        // t = 1.1
        // S = 10 * 1 / 2 +
        //     10 * (1.1 - 1) = 5 + 1 = 6
        EXPECT_FLOAT_EQ(6, particle.calculateDistance(TimeValue(1, 100)));
    }
}

