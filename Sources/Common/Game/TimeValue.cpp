#include "TimeValue.hpp"

using namespace Common::Game;

TimeValue::TimeValue() :
    m_seconds(0),
    m_miliseconds(0)
{
}

TimeValue::TimeValue(unsigned seconds, unsigned miliseconds) :
    m_seconds(seconds),
    m_miliseconds(miliseconds)
{
}

unsigned TimeValue::getSeconds() const
{
    return m_seconds;
}

unsigned TimeValue::getMiliseconds() const
{
    return m_miliseconds;
}

bool TimeValue::operator==(const TimeValue & val) const
{
    return m_seconds == val.m_seconds && m_miliseconds == val.m_miliseconds;
}

TimeValue TimeValue::operator-(const TimeValue & val) const
{
    return TimeValue(m_miliseconds >= val.m_miliseconds ? m_seconds - val.m_seconds: m_seconds - val.m_seconds - 1 ,
        m_miliseconds >= val.m_miliseconds ? m_miliseconds - val.m_miliseconds: (1000 + m_miliseconds) - val.m_miliseconds );
}

TimeValue TimeValue::operator+(const TimeValue & val) const
{
    unsigned seconds = m_seconds + val.m_seconds;
    unsigned miliseconds = m_miliseconds + val.m_miliseconds;

    if (miliseconds >= 1000)
    {
        seconds++;
        miliseconds -= 1000;
    }

    return TimeValue(seconds, miliseconds);
}

bool TimeValue::operator<(const TimeValue & val) const
{
    if (m_seconds == val.m_seconds)
    {
        return m_miliseconds < val.m_miliseconds;
    }
    return m_seconds < val.m_seconds;
}

bool TimeValue::operator<=(const TimeValue & val) const
{
    return *this == val ||  *this < val;
}

