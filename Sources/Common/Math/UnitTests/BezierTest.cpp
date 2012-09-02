#include <gtest/gtest.h>

#include "Common/Math/Bezier.hpp"

TEST(BezierTest, QuadricCurve)
{
	Common::Bezier<Common::Point3<int>, double> b1;

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
	Common::Bezier<Common::Point3<int>, double> b1;

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

