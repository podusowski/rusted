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
    float maxSpeed = 2.0;
    float acceleration = 1.0;
    float targetDistance = 8.0;

    KinematicParticle particle(maxSpeed, acceleration, targetDistance);

    EXPECT_EQ(TimeValue(6, 0), particle.getEta(TimeValue()));

    // acceleration phase
    {
        EXPECT_FLOAT_EQ(0, particle.calculateDistance(TimeValue(0, 0)));

        EXPECT_FLOAT_EQ(0.5, particle.calculateDistance(TimeValue(1, 0)));
        EXPECT_FLOAT_EQ(1, particle.calculateSpeed(TimeValue(1, 0)));

        EXPECT_FLOAT_EQ(2, particle.calculateDistance(TimeValue(2, 0)));
        EXPECT_FLOAT_EQ(2, particle.calculateSpeed(TimeValue(2, 0)));
    }

    // steady phase
    {
        EXPECT_FLOAT_EQ(4, particle.calculateDistance(TimeValue(3, 0)));
        EXPECT_FLOAT_EQ(2, particle.calculateSpeed(TimeValue(3, 0)));

        EXPECT_FLOAT_EQ(6, particle.calculateDistance(TimeValue(4, 0)));
        EXPECT_FLOAT_EQ(2, particle.calculateSpeed(TimeValue(4, 0)));
    }

    // deceleration phase
    {
        EXPECT_FLOAT_EQ(7.5, particle.calculateDistance(TimeValue(5, 0)));
        EXPECT_FLOAT_EQ(1, particle.calculateSpeed(TimeValue(5, 0)));

        EXPECT_FLOAT_EQ(8, particle.calculateDistance(TimeValue(6, 0)));
        EXPECT_FLOAT_EQ(0, particle.calculateSpeed(TimeValue(6, 0)));
    }

    // out of domain
    {
        EXPECT_FLOAT_EQ(8, particle.calculateDistance(TimeValue(7, 0)));
        EXPECT_FLOAT_EQ(0, particle.calculateSpeed(TimeValue(7, 0)));
    }
}

/*
         speed
           ^
  maxSpeed |
           |
           |  /\
           | /. \
           |/ .  \
           '----------> time
              t1 Tmax
*/
TEST(KinematicParticleTest, MovementWithoutMaximumSpeed)
{
    float maxSpeed = 2;
    float acceleration = 1;
    float targetDistance = 2;
    float initialSpeed = 0;

    KinematicParticle particle(maxSpeed, acceleration, targetDistance, initialSpeed);

    EXPECT_EQ(TimeValue(2, 0), particle.getEta(TimeValue(0, 0)));

    // acceleration phase
    {
        EXPECT_FLOAT_EQ(0, particle.calculateDistance(TimeValue(0, 0)));

        /* TODO: ... */

        EXPECT_FLOAT_EQ(0.5, particle.calculateDistance(TimeValue(1, 0)));
        EXPECT_FLOAT_EQ(1, particle.calculateSpeed(TimeValue(1, 0)));
    }

    // deceleration phase
    {
        EXPECT_FLOAT_EQ(0.875, particle.calculateDistance(TimeValue(1, 500)));
        EXPECT_FLOAT_EQ(0.5, particle.calculateSpeed(TimeValue(1, 500)));
    }

    // out of domain
    {
        EXPECT_FLOAT_EQ(1, particle.calculateDistance(TimeValue(3, 0)));
        EXPECT_FLOAT_EQ(0, particle.calculateSpeed(TimeValue(3, 0)));
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
    float maxSpeed = 3;
    float acceleration = 1;
    float targetDistance = 14.5;
    float initialSpeed = 1;

    KinematicParticle particle(maxSpeed, acceleration, targetDistance, initialSpeed);

    // acceleration
    //
    // t1 - as above
    // t1 = (maxSpeed - initialSpeed) / acceleration
    // t1 = (3 - 1) / 1 = 2
    //
    // S = (initialSpeed * t) + (acceleration * t^2) / 2 (done on paper)
    {
        EXPECT_FLOAT_EQ(0, particle.calculateDistance(TimeValue(0, 0)));
        EXPECT_FLOAT_EQ(1.5, particle.calculateDistance(TimeValue(1, 0)));
    }

    // steady
    {
        EXPECT_FLOAT_EQ(4, particle.calculateDistance(TimeValue(2, 0)));
        EXPECT_FLOAT_EQ(7, particle.calculateDistance(TimeValue(3, 0)));
        EXPECT_FLOAT_EQ(10, particle.calculateDistance(TimeValue(4, 0)));
    }

    // deceleration
    {
        EXPECT_FLOAT_EQ(12.5, particle.calculateDistance(TimeValue(5, 0)));
        EXPECT_FLOAT_EQ(14, particle.calculateDistance(TimeValue(6, 0)));
        EXPECT_FLOAT_EQ(14.5, particle.calculateDistance(TimeValue(7, 0)));
    }

    // out of domain
    {
        EXPECT_FLOAT_EQ(14.5, particle.calculateDistance(TimeValue(8, 0)));
    }
}

TEST(KinematicParticleTest, IsInRange)
{
    float maxSpeed = 3;
    float acceleration = 1;
    float targetDistance = 14.5;
    float initialSpeed = 1;

    KinematicParticle particle(maxSpeed, acceleration, targetDistance, initialSpeed);

    EXPECT_TRUE(particle.isInRange(TimeValue(0, 0)));
    EXPECT_TRUE(particle.isInRange(TimeValue(1, 0)));
    EXPECT_TRUE(particle.isInRange(TimeValue(5, 0)));
    EXPECT_TRUE(particle.isInRange(TimeValue(7, 0)));
    EXPECT_FALSE(particle.isInRange(TimeValue(7, 1)));
}

