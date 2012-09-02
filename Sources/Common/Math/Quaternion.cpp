#include "Quaternion.hpp"

using namespace Common::Math;

Quaternion::Quaternion() : m_w(0), m_x(0), m_y(0), m_z(0)
{
}

Quaternion::Quaternion(float radians, const Point3<int> & axis)
{
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

