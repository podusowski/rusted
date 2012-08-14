#pragma once

#include <set>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "Common/Game/IRustedTime.hpp"
#include "Cake/Threading/Thread.hpp"
#include "Cake/Threading/Mutex.hpp"
#include "Cake/Threading/ConditionVariable.hpp"

namespace Common 
{
namespace Game 
{

class RustedTime : public IRustedTime, public Cake::Threading::IRunnable
{
public:
    RustedTime();
    boost::posix_time::ptime getRustedEpoch();
    unsigned getSeconds();
    void setReferenceTime(unsigned);
    TimeValue getCurrentTime();
    void createTimer(TimeValue, boost::function<void()>);
    void run();

private:
    struct Timer
    {
        boost::function<void()> callback;
        TimeValue expiration;

        bool operator<(const Timer & t) const
        {
            return expiration < t.expiration;
        }
    };

    boost::posix_time::ptime now();

    boost::posix_time::ptime m_epoch;
    Cake::Threading::Thread m_timerThread;
    std::set<Timer> m_timers;
    Cake::Threading::Mutex m_timersMutex;
    Cake::Threading::ConditionVariable m_timersCondition;
};

}
}

