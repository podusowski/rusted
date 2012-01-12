#include <iostream>

#include "Cake/Diagnostics/Logger.hpp"
#include "Common/Game/RustedTime.hpp"

using namespace Common::Game;

RustedTime::RustedTime() : m_epoch(boost::date_time::second_clock<boost::posix_time::ptime>::local_time())
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
