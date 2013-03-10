#include <cmath>
#include <cassert>

#include "Quaternion.hpp"

using namespace Common::Math;

Quaternion::Quaternion() : m_w(0), m_x(0), m_y(1), m_z(0)
{
}

Quaternion::Quaternion(const Matrix3 & rotationMatrix)
{
    // taken from Ogre which describes it as:
    //     Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
    //     article "Quaternion Calculus and Fast Animation".

    const Matrix3 & kRot = rotationMatrix;

    Real fTrace = kRot(0, 0) + kRot(1, 1) + kRot(2, 2);
    Real fRoot;

    if ( fTrace > 0.0 )
    {   
        // |w| > 1/2, may as well choose w > 1/2
        fRoot = sqrt(fTrace + 1.0f);  // 2w
        m_w = 0.5f*fRoot;
        fRoot = 0.5f/fRoot;  // 1/(4w)
        m_x = (kRot(2, 1)-kRot(1, 2))*fRoot;
        m_y = (kRot(0, 2)-kRot(2, 0))*fRoot;
        m_z = (kRot(1, 0)-kRot(0, 1))*fRoot;
    }   
    else
    {
        // |w| <= 1/2
        static size_t s_iNext[3] = { 1, 2, 0 };
        size_t i = 0;
        if ( kRot(1, 1) > kRot(0, 0) )
            i = 1;
        if ( kRot(2, 2) > kRot(i, i) )
            i = 2;
        size_t j = s_iNext[i];
        size_t k = s_iNext[j];

        fRoot = sqrt(kRot(i, i) - kRot(j, j) - kRot(k, k) + 1.0f);
        Real* apkQuat[3] = { &m_x, &m_y, &m_z };
        *apkQuat[i] = 0.5f*fRoot;
        fRoot = 0.5f/fRoot;
        m_w = (kRot(k, j) - kRot(j, k)) * fRoot;
        *apkQuat[j] = (kRot(j, i) + kRot(i, j)) * fRoot;
        *apkQuat[k] = (kRot(k, i) + kRot(i, k)) * fRoot;
    }  
}

Quaternion::Quaternion(const std::tuple<Real, Real, Real> & direction)
{
    Real dirX = std::get<0>(direction);
    Real dirY = std::get<1>(direction);
    Real dirZ = std::get<2>(direction);
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

    assert(!std::isnan(factor));

    *this *= factor;
}

float Quaternion::getW()
{
    return m_w;
}

void Quaternion::setX(Real v)
{
    m_x = v;
}

float Quaternion::getX()
{
    return m_x;
}

void Quaternion::setY(Real v)
{
    m_y = v;
}

float Quaternion::getY()
{
    return m_y;
}

void Quaternion::setZ(Real v)
{
    m_z = v;
}

float Quaternion::getZ()
{
    return m_z;
}

