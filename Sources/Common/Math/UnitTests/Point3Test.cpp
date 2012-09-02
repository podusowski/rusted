#include <gtest/gtest.h>

#include "Common/Math/Point3.hpp"

typedef Common::Point3<int>		point3i;
typedef Common::Point3<float>	point3f;

TEST(Point3Test, Operators)
{
	point3i p1, p2, p3;

	p1 = point3i(1, 2, 4);
	p2 = point3i(4, 5, 0);

	// init/operator =
	EXPECT_TRUE(point3i(1, 2, 4) == p1);
	EXPECT_TRUE(point3i(4, 5, 0) == p2);

	p3 = p1 + p2;

	// operator+
	EXPECT_TRUE(point3i(5, 7, 4) == p3);

	p2 = p3 - p1;

	// operator -
	EXPECT_TRUE(point3i(4, 5, 0) == p2);

	p2 += point3i(1, 1, 1);

	// operator +=
	EXPECT_TRUE(point3i(5, 6, 1) == p2);

	p2 -= point3i(2, 2, 2);

	// operator -=
	EXPECT_TRUE(point3i(3, 4, -1) == p2);

	p2 = p1 * 3;

	// operator *
	EXPECT_TRUE(point3i(3, 6, 12) == p2);

	p2 *= -2;

	// operator *=
	EXPECT_TRUE(point3i(-6, -12, -24) == p2);
}

TEST(Point3Test, distanceTest)
{
	int integerDistance = point3i::distance(point3i(0, 0, 0), point3i(-1, 240, -100));
	float floatDistance = point3f::distance(point3f(2, 1, -4), point3f(-4, 3, 10));

	EXPECT_EQ(260, integerDistance);
	EXPECT_EQ(15.362291f, floatDistance);
}

TEST(Point3Test, FloatMultiply)
{
    point3i p1(2, 20, 200);
    point3i p2(2, 20, 200);

    p1 = p1 * .5f;
    p2 *= .5f;

    EXPECT_EQ(1, p1.getX());
    EXPECT_EQ(10, p1.getY());
    EXPECT_EQ(100, p1.getZ());

    EXPECT_TRUE(p1 == p2);
}

TEST(Point3Test, Length)
{
    point3i p1(0, 0, 1);
    EXPECT_EQ(1, p1.length());
}

TEST(Point3Test, Normalize)
{
    point3i p1(0, 0, 10);
    point3i i = p1.normalize();
    EXPECT_EQ(0, i.getX());
    EXPECT_EQ(0, i.getY());
    EXPECT_EQ(1, i.getZ());
}

