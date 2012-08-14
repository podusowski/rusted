#pragma once

#include <ostream>

namespace Common
{
namespace Game
{

class TimeValue
{
public:
    TimeValue() :
        m_seconds(0),
        m_miliseconds(0)
    {
    }

    TimeValue(unsigned seconds, unsigned miliseconds) :
        m_seconds(seconds),
        m_miliseconds(miliseconds)
    {
    }

    unsigned getSeconds() const
    {
        return m_seconds;
    }

    unsigned getMiliseconds() const
    {
        return m_miliseconds;
    }

    bool operator==(const TimeValue & val) const
    {
        return m_seconds == val.m_seconds && m_miliseconds == val.m_miliseconds;
    }

    TimeValue operator-(const TimeValue & val)
    {
        return TimeValue(m_miliseconds >= val.m_miliseconds ? m_seconds - val.m_seconds: m_seconds - val.m_seconds - 1 ,
                        m_miliseconds >= val.m_miliseconds ? m_miliseconds - val.m_miliseconds: (1000 + m_miliseconds) - val.m_miliseconds );

    }

    bool operator<(const TimeValue & val) const
    {
        if (m_seconds == val.m_seconds)
        {
            return m_miliseconds < val.m_miliseconds;
        }
        return m_seconds < val.m_seconds;
    }

private:
    unsigned m_seconds;
    unsigned m_miliseconds;
};

}
}

inline std::ostream & operator<<(std::ostream & os, const Common::Game::TimeValue & val)
{
    return os << val.getSeconds() << "." << val.getMiliseconds() << "s";
}
