#pragma once

#include <tuple>
#include <ostream>

#include "Real.hpp"

namespace Common
{
namespace Math
{

class Quaternion
{
public:
    Quaternion();
    Quaternion(Real radians, const std::tuple<int, int, int> &);
    Quaternion(Real w, Real x, Real y, Real z);
    ~Quaternion();

    bool operator == (const Quaternion &) const;
    Quaternion & operator *= (Real factor);

    void normalize();

    float getW();
    float getX();
    float getY();
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

