#include <boost/date_time/time_clock.hpp>
#include <gtest/gtest.h>

#include <boost/date_time/posix_time/posix_time.hpp>

#include "Common/Game/RustedTime.hpp"

TEST(RustedTimeTest, RuntimeAsserts)
{
    Common::Game::RustedTime time;
    time.getCurrentTime();
}
