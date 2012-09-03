#include "Quaternion.hpp"

using namespace Common::Math;

Quaternion::Quaternion() : m_w(0), m_x(0), m_y(1), m_z(0)
{
}

Quaternion::Quaternion(float radians, const Point3<int> & axis)
{
    float halfAngle = 0.5 * radians;
    float s = sin(halfAngle);
    m_w = cos(halfAngle);
    m_x = s * axis.getX();
    m_y = s * axis.getY();
    m_z = s * axis.getZ();
}

Quaternion::~Quaternion()
{
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

