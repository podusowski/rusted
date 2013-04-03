#pragma once

#include "Real.hpp"

namespace Common
{
namespace Math
{

class Degree
{
public:
    Degree(Real value) : m_value(value)
    {
    }

    Degree & operator = (Real value)
    {
        m_value = value;
        return *this;
    }

    Real operator * () const
    {
        return m_value;
    }

    void normalize()
    {
        if (m_value > 180)
        {
            m_value = - (180 - (m_value - 180));
        }
        else if (m_value < -180)
        {
            m_value = 180 + (m_value + 180);
        }
    }

private:
    Real m_value;
};

}
}

