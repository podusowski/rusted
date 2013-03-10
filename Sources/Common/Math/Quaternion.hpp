#pragma once

#include <tuple>
#include <ostream>

#include "Real.hpp"
#include "Matrix3.hpp"

namespace Common
{
namespace Math
{

class Quaternion
{
public:
    Quaternion();
    Quaternion(const Matrix3 & rotationMatrix);
    Quaternion(const std::tuple<Real, Real, Real> & direction);
    Quaternion(Real radians, const std::tuple<int, int, int> &);
    Quaternion(Real w, Real x, Real y, Real z);
    ~Quaternion();

    bool operator == (const Quaternion &) const;
    Quaternion & operator *= (Real factor);

    void normalize();

    float getW();

    void setX(Real);
    float getX();

    void setY(Real);
    float getY();

    void setZ(Real);
    float getZ();

private:
    float m_w;
    float m_x;
    float m_y;
    float m_z;
};

}
}

inline std::ostream & operator<<(std::ostream & os, Common::Math::Quaternion q)
{
    return os << "(" << q.getW() << ", " << q.getX() << ", " << q.getY() << ", " << q.getZ() << ")";
}

