#include <cmath>

#include "Quaternion.hpp"

using namespace Common::Math;

Quaternion::Quaternion() : m_w(0), m_x(0), m_y(1), m_z(0)
{
}

Quaternion::Quaternion(float radians, const std::tuple<int, int, int> & axis)
{
    float halfAngle = 0.5 * radians;
    float s = sin(halfAngle);
    m_w = cos(halfAngle);
    m_x = s * std::get<0>(axis);
    m_y = s * std::get<1>(axis);
    m_z = s * std::get<2>(axis);
}

Quaternion::Quaternion(Real w, Real x, Real y, Real z) :
    m_w(w), m_x(x), m_y(y), m_z(z)
{
}

Quaternion::~Quaternion()
{
}

bool Quaternion::operator == (const Quaternion & rhs) const
{
    return m_w == rhs.m_w && m_x == rhs.m_x && m_y == rhs.m_y && m_z == rhs.m_z;
}

Quaternion & Quaternion::operator *= (Real factor)
{
    m_w *= factor;
    m_x *= factor;
    m_y *= factor;
    m_z *= factor;

    return *this;
}

void Quaternion::normalize()
{
    Real len = m_w * m_w + m_x * m_x + m_y * m_y + m_z * m_z;
    Real factor = 1.0 / len;
    *this *= factor;
}

float Quaternion::getW()
{
    return m_w;
}

float Quaternion::getX()
{
    return m_x;
}

float Quaternion::getY()
{
    return m_y;
}

float Quaternion::getZ()
{
    return m_z;
}

