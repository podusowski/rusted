#pragma once

#include "Point3.hpp"

namespace Common
{
namespace Math
{

class Quaternion
{
public:
    Quaternion();
    Quaternion(float radians, const Point3<int> & axis);
    ~Quaternion();

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
