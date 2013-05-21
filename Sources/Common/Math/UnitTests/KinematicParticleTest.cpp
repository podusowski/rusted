#include <gtest/gtest.h>

#include "Common/Math/KinematicParticle.hpp"

using namespace Common::Math;
using Common::Game::TimeValue;

/*
         speed
           ^
           |
  maxSpeed |   ------------
           |  /            \
           | /.     S      .\
           |/ .            . \
           '----------------------> time
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
        EXPECT_FLOAT_EQ(0, particle.calculateDistance(TimeValue(0, 0)));

        EXPECT_FLOAT_EQ(0.05, particle.calculateDistance(TimeValue(0, 100)));
        EXPECT_FLOAT_EQ(1, particle.calculateSpeed(TimeValue(0, 100)));

        EXPECT_FLOAT_EQ(1.25, particle.calculateDistance(TimeValue(0, 500)));
        EXPECT_FLOAT_EQ(5, particle.calculateSpeed(TimeValue(0, 500)));
    }

    // steady phase
    //
    // t1 - time when particle stops accelerating and has maximum speed
    // t1 = maxSpeed / acceleration
    // t1 = 1
    // S = (maxSpeed * t1) / 2 + // triangle
    //     maxSpeed * (t - t1)   // rect
    // V = maxSpeed
    {
        // t = 1.1
        // S = 10 * 1 / 2 +
        //     10 * (1.1 - 1) = 5 + 1 = 6
        EXPECT_FLOAT_EQ(6, particle.calculateDistance(TimeValue(1, 100)));
        EXPECT_FLOAT_EQ(10, particle.calculateSpeed(TimeValue(1, 100)));
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
    // V = (Tmax - t) * a
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
        EXPECT_FLOAT_EQ(9, particle.calculateSpeed(TimeValue(10, 100)));

        // Tmax check
        EXPECT_FLOAT_EQ(100, particle.calculateDistance(TimeValue(11, 0)));
    }
}

/*
              speed
                ^
                |
       maxSpeed | ------------
                |/            \
   initialSpeed |.     S      .\
                |.            . \
                '--------------------> time
                 t1           t2 Tmax
*/
TEST(KinematicParticleTest, MovementWithInitialSpeed)
{
    float maxSpeed = 10;
    float acceleration = 10;
    float targetDistance = 100;
    float initialSpeed = 5;

    KinematicParticle particle(maxSpeed, acceleration, targetDistance, initialSpeed);

    // acceleration
    //
    // t1 - as above
    // t1 = (maxSpeed - initialSpeed) / acceleration
    // t1 = 5 / 10 = 0.5
    //
    // S = (initialSpeed * t) + (acceleration * t^2) / 2 (done on paper)
    {
        EXPECT_FLOAT_EQ(0, particle.calculateDistance(TimeValue(0, 0)));
        //EXPECT_FLOAT_EQ(5, particle.calculateSpeed(TimeValue(0, 0)));

        // t = 0.1
        // S = (5 * 0.1) + (10 * 0.1^2) / 2
        //   = 0.5 + (10 * 0.01) / 2
        //   = 0.5 + 0.1 / 2
        //   = 0.5 + 0.05
        //   = 0.55
        EXPECT_FLOAT_EQ(0.55, particle.calculateDistance(TimeValue(0, 100)));
    }

    // steady
    //
    // S = initialSpeed * t1
    //     + (acceleration * t1^2) / 2
    //     + maxSpeed * (t - t1)
    {
        // t = 1
        // S = (5 * 0.5)
        //     + (10 * 0.5^2) / 2
        //     + 10 * (1 - 0.5)
        //
        //   = 2.5
        //     + 10 * 0.25 / 2
        //     + 10 * 0.5
        //
        //   = 2.5
        //     + 2.5 / 2
        //     + 5
        //
        //   = 2.5
        //     + 1.25
        //     + 5
        //
        //   = 7.5 + 1.25
        //   = 8.75
        EXPECT_FLOAT_EQ(8.75, particle.calculateDistance(TimeValue(1, 0)));

        // V = maxSpeed = 10
        EXPECT_FLOAT_EQ(10, particle.calculateSpeed(TimeValue(1, 0)));
    }

    // deceleration
    //
    // S = (initialSpeed * t1) + [(acceleration * t1^2) / 2] +   // first triangle
    //     (maxSpeed * (t2 - t1)) +                            // middle rectangle
    //     (maxSpeed * (Tmax - t2)) / 2                        // second triangle
    //     - [acceleration * ((Tmax - t) ^ 2)] / 2                 // minus small right triangle
    //
    // Tmax = (targetDistance / maxSpeed)
    //        - (t1 * initialSpeed) / (2 * maxSpeed)
    //        + t1 / 2
    //
    // Tmax = 10.125
    //
    // t2 = Tmax - maxSpeed/acceleration
    // t2 = 10.125 - 10/10
    //    = 9.125
    {
        // t = 10
        // S = [5 * 0.5] + [(10 * 0.25)/2]
        //     + 10 * (9.125 - 0.5)
        //     + [10 * (10.125 - 9.125)] / 2
        //     - [10 * (10.125 - 10)^2] / 2
        //   = 2.5 + (2.5 / 2)
        //     + 10 * 8.625
        //     + 10 / 2

        // Tmax check
        EXPECT_FLOAT_EQ(100, particle.calculateDistance(TimeValue(10, 95)));
    }
}

TEST(KinematicParticleTest, IsInRange)
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
    EXPECT_ANY_THROW(particle.calculateSpeed(TimeValue(11, 1)));
}

