#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "Point3.hpp"

class Point3Test : public CPPUNIT_NS :: TestFixture
{
	CPPUNIT_TEST_SUITE (Point3Test);

	CPPUNIT_TEST(operatorsTest);
	CPPUNIT_TEST(distanceTest);
	CPPUNIT_TEST(testFloatMultiply);
	CPPUNIT_TEST(testLength);
	CPPUNIT_TEST(testNormalize);

	CPPUNIT_TEST_SUITE_END ();

protected:
    void operatorsTest();
    void distanceTest();
    void testFloatMultiply();
    void testLength();
    void testNormalize();
};

CPPUNIT_TEST_SUITE_REGISTRATION (Point3Test);

typedef Common::Point3<int>		point3i;
typedef Common::Point3<float>	point3f;

void Point3Test::operatorsTest()
{
	point3i p1, p2, p3;

	p1 = point3i(1, 2, 4);
	p2 = point3i(4, 5, 0);

	// init/operator =
	CPPUNIT_ASSERT(point3i(1, 2, 4) == p1);
	CPPUNIT_ASSERT(point3i(4, 5, 0) == p2);

	p3 = p1 + p2;

	// operator+
	CPPUNIT_ASSERT(point3i(5, 7, 4) == p3);

	p2 = p3 - p1;

	// operator -
	CPPUNIT_ASSERT(point3i(4, 5, 0) == p2);

	p2 += point3i(1, 1, 1);

	// operator +=
	CPPUNIT_ASSERT(point3i(5, 6, 1) == p2);

	p2 -= point3i(2, 2, 2);

	// operator -=
	CPPUNIT_ASSERT(point3i(3, 4, -1) == p2);

	p2 = p1 * 3;

	// operator *
	CPPUNIT_ASSERT(point3i(3, 6, 12) == p2);

	p2 *= -2;

	// operator *=
	CPPUNIT_ASSERT(point3i(-6, -12, -24) == p2);
}

void Point3Test::distanceTest()
{
	int integerDistance = point3i::distance(point3i(0, 0, 0), point3i(-1, 240, -100));
	float floatDistance = point3f::distance(point3f(2, 1, -4), point3f(-4, 3, 10));

	CPPUNIT_ASSERT_EQUAL(260, integerDistance);
	CPPUNIT_ASSERT_EQUAL(15.362291f, floatDistance);
}

void Point3Test::testFloatMultiply()
{
    point3i p1(2, 20, 200);
    point3i p2(2, 20, 200);

    p1 = p1 * .5f;
    p2 *= .5f;

    CPPUNIT_ASSERT_EQUAL(1, p1.getX());
    CPPUNIT_ASSERT_EQUAL(10, p1.getY());
    CPPUNIT_ASSERT_EQUAL(100, p1.getZ());

    CPPUNIT_ASSERT(p1 == p2);
}

void Point3Test::testLength()
{
    point3i p1(0, 0, 1);
    CPPUNIT_ASSERT_EQUAL(1, p1.length());
}

void Point3Test::testNormalize()
{
    point3i p1(0, 0, 10);
    point3i i = p1.normalize();
    CPPUNIT_ASSERT_EQUAL(0, i.getX());
    CPPUNIT_ASSERT_EQUAL(0, i.getY());
    CPPUNIT_ASSERT_EQUAL(1, i.getZ());
}

