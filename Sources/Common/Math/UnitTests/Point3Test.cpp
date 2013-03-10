#include <gtest/gtest.h>

#include "Common/Math/Point3.hpp"

using namespace Common::Math;

TEST(Point3Test, Operators)
{
	Point3 p1, p2, p3;

	p1 = Point3(1, 2, 4);
	p2 = Point3(4, 5, 0);

	// init/operator =
	EXPECT_TRUE(Point3(1, 2, 4) == p1);
	EXPECT_TRUE(Point3(4, 5, 0) == p2);

	p3 = p1 + p2;

	// operator+
	EXPECT_TRUE(Point3(5, 7, 4) == p3);

	p2 = p3 - p1;

	// operator -
	EXPECT_TRUE(Point3(4, 5, 0) == p2);

	p2 += Point3(1, 1, 1);

	// operator +=
	EXPECT_TRUE(Point3(5, 6, 1) == p2);

	p2 -= Point3(2, 2, 2);

	// operator -=
	EXPECT_TRUE(Point3(3, 4, -1) == p2);

	p2 = p1 * 3;

	// operator *
	EXPECT_TRUE(Point3(3, 6, 12) == p2);

	p2 *= -2;

	// operator *=
	EXPECT_TRUE(Point3(-6, -12, -24) == p2);
}

TEST(Point3Test, distanceTest)
{
	float floatDistance = Point3::distance(Point3(2, 1, -4), Point3(-4, 3, 10));
	EXPECT_EQ(15.362291f, floatDistance);
}

TEST(Point3Test, FloatMultiply)
{
    Point3 p1(2, 20, 200);
    Point3 p2(2, 20, 200);

    p1 = p1 * .5f;
    p2 *= .5f;

    EXPECT_EQ(1, p1.getX());
    EXPECT_EQ(10, p1.getY());
    EXPECT_EQ(100, p1.getZ());

    EXPECT_TRUE(p1 == p2);
}

TEST(Point3Test, Length)
{
    Point3 p1(0, 0, 1);
    EXPECT_EQ(1, p1.length());
}

TEST(Point3Test, Normalize)
{
    Point3 p1(0, 0, 10);
    p1.normalize();
    EXPECT_EQ(0, p1.getX());
    EXPECT_EQ(0, p1.getY());
    EXPECT_EQ(1, p1.getZ());
}

TEST(Point3Test, RotationTo)
{
    Point3 p1(0, 0, 10);
    Point3 p2(0, 0, 0);

    // 90 degrees
    // TODO
    //EXPECT_EQ(Quaternion(PI / 2, std::make_tuple(0, 1, 0)), p1.getRotationTo(p2));
}

