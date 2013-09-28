#include <iostream>

#include "Cake/Diagnostics/Logger.hpp"
#include "Cake/Threading/ScopedLock.hpp"
#include "Common/Game/RustedTime.hpp"

using namespace Common::Game;

RustedTime::RustedTime() :
    m_epoch(now()), 
    m_timerThread(*this),
    m_timersCondition(m_timersMutex),
    m_finishing(false)
{
    LOG_INFO << "Rusted epoch set to " << m_epoch;
}

RustedTime::~RustedTime()
{
    if (m_timerThread.isRunning())
    {
        LOG_DEBUG << "Waiting for timer thread to finish";

        m_finishing = true;
        m_timersCondition.signal();
        while (m_finishing) ;
    }
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

void RustedTime::setReferenceTime(TimeValue reference)
{
    unsigned now = getSeconds();
    m_epoch += boost::posix_time::seconds(now) 
                - boost::posix_time::seconds(reference.getSeconds())
                - boost::posix_time::milliseconds(reference.getMiliseconds());

    LOG_INFO << "New epoch set to " << m_epoch;
}

void RustedTime::createTimer(TimeValue time, boost::function<void()> callback)
{
    unsigned id = m_idGenerator.generate();

    Timer t(id);
    t.callback = callback;
    t.expiration = getCurrentTime() + time;

    LOG_DEBUG << "Starting timer, id:" << id << ", timeout:" << time << ", there are " << m_timers.size() << " active timers";

    Cake::Threading::ScopedLock lock(m_timersMutex);

    if (!m_timerThread.isRunning())
    {
        LOG_DEBUG << "Starting timer thread";
        m_timerThread.start();
    }

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

TimeValue RustedTime::getAbsoluteTime()
{
    boost::posix_time::ptime t = now();
    boost::posix_time::ptime posixEpoch(boost::gregorian::date(1970, boost::gregorian::Jan, 1));

    auto duration = t - posixEpoch;

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
    while (!m_finishing)
    {
        m_timersMutex.aquire();
        if (m_timers.empty())
        {
            LOG_DEBUG << "Timer queue is empty, waiting";
            m_timersCondition.wait();
        }

        if (!m_timers.empty())
        {
            Timer firstTimer = *m_timers.begin();
            TimeValue t = getCurrentTime();

            if (firstTimer.expiration <= t)
            {
                LOG_DEBUG << "Timer id:" << firstTimer.m_id << " expired";

                m_timersMutex.release();

                try
                {
                    firstTimer.callback();
                }
                catch (std::exception & ex)
                {
                    LOG_WARN << "Exception during timer callback: " << ex.what();
                }

                m_timersMutex.aquire();

                m_timers.erase(m_timers.begin());
            }
            else
            {
                TimeValue timeToWait = firstTimer.expiration - t;
                LOG_DEBUG << "Timer thread as awake but there is no expiration yet, waiting:" << timeToWait << ", now:" << getCurrentTime();

                m_timersCondition.timedWait(timeToWait.getSeconds(), timeToWait.getMiliseconds());
            }
        }

        m_timersMutex.release();
    }

    LOG_DEBUG << "Timer thread finished";

    m_finishing = false;
}

