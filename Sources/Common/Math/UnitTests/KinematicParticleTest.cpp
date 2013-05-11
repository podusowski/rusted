#include <gtest/gtest.h>

#include "Common/Math/KinematicParticle.hpp"

using namespace Common::Math;
using Common::Game::TimeValue;

/*

         speed
           A
  maxSpeed |   ____________
           |  /            \
           | /.     S      .\
           |/ .            . \
           +----------------------> time
              t1           t2 Tmax

*/

TEST(KinematicParticleTest, FullMovement)
{
    float maxSpeed = 10.0;
    float acceleration = 10.0;
    float targetDistance = 100.0;

    KinematicParticle particle(maxSpeed, acceleration, targetDistance);

    // acceleration phase
    //
    // S = at2/2
    // V = at
    {
        EXPECT_FLOAT_EQ(0.05, particle.calculateDistance(TimeValue(0, 100)));
        EXPECT_FLOAT_EQ(1.25, particle.calculateDistance(TimeValue(0, 500)));
    }

    // steady phase
    //
    // t1 - time when particle stops accelerating and has maximum speed
    // t1 = maxSpeed / acceleration
    // t1 = 1
    // S = (maxSpeed * t1) / 2 + // triangle
    //     maxSpeed * (t - t1)   // rect
    {
        // t = 1.1
        // S = 10 * 1 / 2 +
        //     10 * (1.1 - 1) = 5 + 1 = 6
        EXPECT_FLOAT_EQ(6, particle.calculateDistance(TimeValue(1, 100)));
    }

    // deceleration phase
    //
    // Tmax - total time which targetDistance should be traveled in
    // Tmax = (targetDistance / maxSpeed) + (maxSpeed / acceleration)
    // Tmax = 11
    // t2 - time when particle starts to decelerate in order to met targetDistance
    // t2 = Tmax - t1
    // t2 = 10
    //
    // S = (maxSpeed * t1) +     // 2 triangles
    //     maxSpeed * (t2 - t1)  // rect
    //     - acceleration * (Tmax - t) ^ 2 / 2 // minus small right triangle
    {
        // t = 10.1
        // S = 10 * 1 +
        //     10 * 9 -
        //     10 * (0.9^2) / 2
        //   = 100 - (10 * 0.81) / 2
        //   = 100 - 8.1 / 2
        //   = 100 - 4.05
        //   = 95.95
        EXPECT_FLOAT_EQ(95.95, particle.calculateDistance(TimeValue(10, 100)));
    }
}

TEST(KinematicParticle, IsInRange)
{
    float maxSpeed = 10;
    float acceleration = 10;
    float targetDistance = 100;

    KinematicParticle particle(maxSpeed, acceleration, targetDistance);

    // see above for formula
    // Tmax = 11

    EXPECT_TRUE(particle.isInRange(TimeValue(0, 0)));
    EXPECT_TRUE(particle.isInRange(TimeValue(1, 0)));
    EXPECT_TRUE(particle.isInRange(TimeValue(5, 0)));
    EXPECT_TRUE(particle.isInRange(TimeValue(10, 0)));
    EXPECT_TRUE(particle.isInRange(TimeValue(11, 0)));
    EXPECT_FALSE(particle.isInRange(TimeValue(11, 1)));
    EXPECT_ANY_THROW(particle.calculateDistance(TimeValue(11, 1)));
}

