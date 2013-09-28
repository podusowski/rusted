#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <boost/bind.hpp>

#include "Common/Game/RustedTime.hpp"
#include "Cake/Diagnostics/Logger.hpp"

using namespace testing;
using namespace Common::Game;

TEST(RustedTimeTest, RuntimeAsserts)
{
    Common::Game::RustedTime time;
    time.getCurrentTime();
}

struct TimeoutValidator
{
public:
    TimeoutValidator(Common::Game::IRustedTime & time) : m_time(time)
    {
    }

    void operator()(unsigned timeout)
    {
        auto now = m_time.getCurrentTime();
        EXPECT_NEAR(timeout, now.getMiliseconds(), 100);
    }

private:
    Common::Game::IRustedTime & m_time;
};

struct TimerCallbackMock
{
    MOCK_METHOD1(expired, void(unsigned));
};

TEST(RustedTimeTest, SafeDestructor)
{
    bool b = false;
    {
        Common::Game::RustedTime time;

        time.createTimer(TimeValue(0, 1), [&]() -> void
        {
            b = true;
        });
    }

    Cake::Threading::Thread::wait(0, 500);

    EXPECT_FALSE(b);
}

TEST(RustedTimeTest, Timers)
{
    Common::Game::RustedTime time;
    TimerCallbackMock callback;
    TimeoutValidator validator(time);

    InSequence s;
    EXPECT_CALL(callback, expired(100)).WillOnce(Invoke(validator));
    EXPECT_CALL(callback, expired(300)).WillOnce(Invoke(validator));
    EXPECT_CALL(callback, expired(410)).WillOnce(Invoke(validator));
    EXPECT_CALL(callback, expired(900)).WillOnce(Invoke(validator));

    time.createTimer(TimeValue(0, 300), boost::bind(&TimerCallbackMock::expired, &callback, 300));
    time.createTimer(TimeValue(0, 410), boost::bind(&TimerCallbackMock::expired, &callback, 410));
    time.createTimer(TimeValue(0, 100), boost::bind(&TimerCallbackMock::expired, &callback, 100));
    time.createTimer(TimeValue(0, 900), boost::bind(&TimerCallbackMock::expired, &callback, 900));

    Cake::Threading::Thread::wait(1, 500);
}

TEST(RustedTimeTest, TimersStress)
{
    Common::Game::RustedTime time;
    TimerCallbackMock callback;
    TimeoutValidator validator(time);

    EXPECT_CALL(callback, expired(400)).Times(100).WillRepeatedly(Invoke(validator));

    for (int i = 0; i < 100; i++)
    {
        time.createTimer(TimeValue(0, 400), boost::bind(&TimerCallbackMock::expired, &callback, 400));
    }

    Cake::Threading::Thread::wait(1, 500);
}

TEST(RustedTimeTest, Bug_DeadlockWhenStartingTimerInExpirationHandler)
{
    Common::Game::RustedTime time;
    TimerCallbackMock callback;

    EXPECT_CALL(callback, expired(_)).Times(1);

    time.createTimer(TimeValue(0, 1), [&]() -> void
    {
        time.createTimer(TimeValue(0, 1), boost::bind(&TimerCallbackMock::expired, &callback, 1));
    });

    // ugly hack to avoid object destruction
    Cake::Threading::Thread::wait(0, 500);
}

