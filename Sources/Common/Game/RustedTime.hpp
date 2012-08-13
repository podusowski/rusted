#pragma once

#include <boost/date_time/posix_time/posix_time.hpp>

#include "Common/Game/IRustedTime.hpp"

namespace Common 
{
namespace Game 
{

class RustedTime : public IRustedTime
{
public:
    RustedTime();
    boost::posix_time::ptime getRustedEpoch();
    unsigned getSeconds();
    void setReferenceTime(unsigned);
    TimeValue getCurrentTime();
    void createTimer(TimeValue, boost::function<void()>);

private:
    boost::posix_time::ptime now();

    boost::posix_time::ptime m_epoch;
};

}
}

