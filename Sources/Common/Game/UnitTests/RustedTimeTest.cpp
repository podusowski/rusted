#include <boost/date_time/time_clock.hpp>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <boost/date_time/posix_time/posix_time.hpp>

#include "Common/Thread.hpp"
#include "Common/Game/RustedTime.hpp"

class RustedTimeTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE (RustedTimeTest);
    CPPUNIT_TEST(testGetRustedEpoch);
    CPPUNIT_TEST(testGetSeconds);
    CPPUNIT_TEST_SUITE_END ();

public:
    void testGetRustedEpoch();
    void testGetSeconds();
};

CPPUNIT_TEST_SUITE_REGISTRATION (RustedTimeTest);

void RustedTimeTest::testGetRustedEpoch()
{
    Common::Game::RustedTime time;

    boost::posix_time::ptime t(boost::date_time::second_clock<boost::posix_time::ptime>::local_time());
    CPPUNIT_ASSERT(t == time.getRustedEpoch());
}

void RustedTimeTest::testGetSeconds()
{
    Common::Game::RustedTime time;

    Common::Thread::wait(1.0);

    CPPUNIT_ASSERT_EQUAL(unsigned(1), time.getSeconds());
}

