#include <iostream>

#include "Cake/Diagnostics/Logger.hpp"
#include "Cake/Threading/ScopedLock.hpp"
#include "Common/Game/RustedTime.hpp"

using namespace Common::Game;

RustedTime::RustedTime() : 
    m_epoch(now()), 
    m_timerThread(*this), 
    m_timersCondition(m_timersMutex)
{
    LOG_INFO << "Rusted epoch set to " << m_epoch;
}

boost::posix_time::ptime RustedTime::getRustedEpoch()
{
   return m_epoch; 
}

unsigned RustedTime::getSeconds()
{
    boost::posix_time::ptime t(boost::date_time::second_clock<boost::posix_time::ptime>::local_time());
    return (t - m_epoch).total_seconds();
}

void RustedTime::setReferenceTime(unsigned reference)
{
    unsigned now = getSeconds();
    m_epoch += boost::posix_time::seconds(now - reference);

    LOG_INFO << "New epoch set to " << m_epoch;
}

void RustedTime::createTimer(TimeValue time, boost::function<void()> callback)
{
    LOG_DEBUG << "Starting timer: " << time;

    Timer t;
    t.callback = callback;
    t.expiration = getCurrentTime() + time;

    if (!m_timerThread.isRunning())
    {
        LOG_DEBUG << "Starting timer thread";
        m_timerThread.start();
    }

    Cake::Threading::ScopedLock lock(m_timersMutex);
    m_timers.insert(t);
    m_timersCondition.signal();
}

TimeValue RustedTime::getCurrentTime()
{
    boost::posix_time::ptime t = now();
    boost::posix_time::time_duration duration = t - m_epoch;

    // actually, we need mili resolution, but since we're too lazy to implement
    // converters, we will stick with micro
    assert(duration.resolution() == boost::date_time::micro);

    unsigned seconds = duration.total_seconds();
    unsigned miliseconds = duration.fractional_seconds() / 1000;

    return TimeValue(seconds, miliseconds);
}

boost::posix_time::ptime RustedTime::now()
{
    return boost::date_time::microsec_clock<boost::posix_time::ptime>::local_time();
}

void RustedTime::run()
{
    while (true)
    {
        m_timersMutex.aquire();
        if (m_timers.empty())
        {
            LOG_DEBUG << "Timer queue is empty, waiting";
            m_timersCondition.wait();
            m_timersMutex.release();
            LOG_DEBUG << "New timer detected";
        }
        else
        {
            LOG_DEBUG << "There are active timers";
            m_timersMutex.release();
        }

        Timer firstTimer;
        {
            Cake::Threading::ScopedLock lock(m_timersMutex);
            LOG_DEBUG << "Getting the first timer";
            firstTimer = *m_timers.begin();
        }

        TimeValue t = getCurrentTime();

        if (firstTimer.expiration < t)
        {
            LOG_DEBUG << "Timer expired";
            firstTimer.callback();

            Cake::Threading::ScopedLock lock(m_timersMutex);
            m_timers.erase(m_timers.begin());
        }
        else
        {
            LOG_DEBUG << "Timer is not expired yet";
            TimeValue timeToWait = firstTimer.expiration - t;
            
            Cake::Threading::ScopedLock lock(m_timersMutex);
            m_timersCondition.timedWait(timeToWait.getSeconds());
        }
    }
}

