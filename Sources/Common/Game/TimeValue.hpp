#pragma once

#include <ostream>

namespace Common
{
namespace Game
{

class TimeValue
{
public:
    TimeValue();
    TimeValue(unsigned seconds, unsigned miliseconds);
    static TimeValue fromSeconds(float seconds);

    unsigned getSeconds() const;
    unsigned getMiliseconds() const;
    bool operator==(const TimeValue & val) const;
    bool operator!=(const TimeValue & val) const;
    TimeValue operator-(const TimeValue & val) const;
    TimeValue operator+(const TimeValue & val) const;
    bool operator<(const TimeValue & val) const;
    bool operator<=(const TimeValue & val) const;

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
