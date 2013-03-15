#pragma once

#include <set>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "Common/Game/IRustedTime.hpp"
#include "Common/Game/Utilities/IdGenerator.hpp"
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
    void setReferenceTime(TimeValue);
    TimeValue getCurrentTime();
    TimeValue getAbsoluteTime();
    void createTimer(TimeValue, boost::function<void()>);
    void run();

private:
    struct Timer
    {
        Timer(unsigned id): m_id(id)
        {
        }

        boost::function<void()> callback;
        TimeValue expiration;
        unsigned m_id;

        bool operator<(const Timer & t) const
        {
            if (expiration == t.expiration)
            {
                return m_id < t.m_id;
            }

            return expiration < t.expiration;
        }
    };

    boost::posix_time::ptime now();

    boost::posix_time::ptime m_epoch;
    Cake::Threading::Thread m_timerThread;
    std::set<Timer> m_timers;
    Cake::Threading::Mutex m_timersMutex;
    Cake::Threading::ConditionVariable m_timersCondition;
    Common::Game::Utilities::IdGenerator m_idGenerator;
};

}
}

