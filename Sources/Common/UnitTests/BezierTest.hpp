#ifndef COMMON_BEZIERTEST_H
#define COMMON_BEZIERTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "Bezier.hpp"

class BezierTest : public CPPUNIT_NS :: TestFixture
{
	CPPUNIT_TEST_SUITE (BezierTest);
		CPPUNIT_TEST (quadricCurveTest);
		CPPUNIT_TEST (cubicCurveTest);
	CPPUNIT_TEST_SUITE_END ();

protected:
        void quadricCurveTest();
        void cubicCurveTest();
};

#endif
