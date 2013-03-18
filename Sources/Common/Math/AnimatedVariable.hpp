#pragma once

#include <cmath>

#include "Real.hpp"

namespace Common
{
namespace Math
{

template<typename T> class AnimatedVariable
{
public:
    AnimatedVariable(T value) :
        m_value(value),
        m_to(value),
        m_progress(0),
        m_speed(0.15),
        m_animating(false)
    {
    }
    
    T operator * () const
    {
        return calculate();
    }

    AnimatedVariable<T> & operator = (T value)
    {
        animateTo(value);
        start();
        return *this;
    }

    AnimatedVariable<T> & operator += (T value)
    {
        animateTo(calculate() + value);
        start();
        return *this;
    }

    AnimatedVariable<T> & operator -= (T value)
    {
        animateTo(calculate() - value);
        start();
        return *this;
    }

    void update()
    {
        m_progress += m_speed;
        if (m_progress >= PI / 2)
        {
            stop();
        }
    }

private:
    void animateTo(T value)
    {
        T now = calculate();
        m_to = value;
        m_value = now;
    }

    T calculate() const
    {
        if (m_animating)
        {
            auto diff = m_to - m_value;
            Real fraction = (sin(m_progress) + 1.0) / 2.0;

            return m_value + (diff * fraction);
        }
        else
        {
            return m_value;
        }
    }

    void start()
    {
        if (m_animating && m_progress < -PI / 4)
        {
            // it's accelerating so just keep it going
        }
        else if (m_animating && m_progress > PI / 4)
        {
            // it's decelerating, so make it accelerate from
            // this point

            m_progress = -m_progress;
        }
        else if (m_animating)
        {
            m_progress = -PI / 4;
        }
        else
        {
            m_progress = -PI / 2;
            m_animating = true;
        }
    }

    void stop()
    {
        if (m_animating)
        {
            m_value = m_to;
            m_animating = false;
        }
    }

    T m_value;
    T m_to;
    Real m_progress;
    Real m_speed;
    bool m_animating;
};

}
}

