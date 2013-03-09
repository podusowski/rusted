#include <gtest/gtest.h>

#include "Common/Math/Bezier3.hpp"

using namespace Common::Math;

//FIXME: for Point3
using namespace Common;

TEST(BezierTest, QuadricCurve)
{
	Common::Math::Bezier3 b1;

	// "random" values :)
	b1.addControlPoint(Common::Point3<int>(4, 10, 12));
	b1.addControlPoint(Common::Point3<int>(21, 80, 3));
	b1.addControlPoint(Common::Point3<int>(100, 130, -7));

	EXPECT_TRUE(Common::Point3<int>(4,10,12) == b1(0));		// check for B(0)
	EXPECT_TRUE(Common::Point3<int>(100,130,-7) == b1(1));	// check for B(1)
	EXPECT_TRUE(Common::Point3<int>(16,43,7) == b1(0.25f));	// check for B(0.25)
	EXPECT_TRUE(Common::Point3<int>(64,103,-2) == b1(0.75f)); // check for B(0.75)
}

TEST(BezierTest, CubicCurveTest)
{
	Common::Math::Bezier3 b1;

	// "random" values :)
	b1.addControlPoint(Common::Point3<int>(0, 0, 0));
	b1.addControlPoint(Common::Point3<int>(1, 20, -2));
	b1.addControlPoint(Common::Point3<int>(8, 311, -7));
	b1.addControlPoint(Common::Point3<int>(100, 728, -40));

	EXPECT_TRUE(Common::Point3<int>(0,0,0) == b1(0));			// check for B(0)
	EXPECT_TRUE(Common::Point3<int>(100,728,-40) == b1(1));	// check for B(1)
	EXPECT_TRUE(Common::Point3<int>(3,63,-2) == b1(0.25f));	// check for B(0.25)
	EXPECT_TRUE(Common::Point3<int>(45,441,-20) == b1(0.75f)); // check for B(0.75)
}

TEST(BezierTest, LinearLength)
{
	Common::Math::Bezier3 b1;

	b1.addControlPoint(Common::Point3<int>(0, 0, 0));
	b1.addControlPoint(Common::Point3<int>(0, 0, 100));

	EXPECT_EQ(100u, b1.getLength());
}

TEST(BezierTest, LinearDerivative)
{
    Common::Math::Bezier3 b1;

    b1.addControlPoint(Common::Point3<int>(0, 0, 0));
    b1.addControlPoint(Common::Point3<int>(0, 0, 100));

    for (float f = 0.0; f < 1.0; f += 0.01)
    {
        EXPECT_EQ(Common::Point3<int>(0, 0, 100), b1.derivative(f));
    }
}

TEST(BezierTest, LinearOrientation)
{
    Common::Math::Bezier3 b1;

    b1.addControlPoint(Common::Point3<int>(0, 0, 0));
    b1.addControlPoint(Common::Point3<int>(0, 0, 100));

    for (float f = 0.0; f < 1.0; f += 0.01)
    {
        //EXPECT_EQ(Common::Math::Quaternion(PI / 4.0, Point3<int>(0, 0, 1)), b1.orientation(f));
    }
}

