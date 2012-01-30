#pragma once

namespace Common
{
namespace Game
{

class TimeValue
{
public:
    TimeValue(unsigned seconds, unsigned miliseconds) :
        m_seconds(seconds),
        m_miliseconds(miliseconds)
    {
    }

    unsigned getSeconds()
    {
        return m_seconds;
    }

    unsigned getMiliseconds()
    {
        return m_miliseconds;
    }

private:
    unsigned m_seconds;
    unsigned m_miliseconds;
};

}
}
