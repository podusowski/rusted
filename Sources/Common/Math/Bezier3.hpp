#pragma once

#include <vector>

#include "Real.hpp"
#include "Point3.hpp"
#include "Quaternion.hpp"

namespace Common
{
namespace Math
{

class Bezier3
{
public:
    typedef Point3<int> PointType;
    typedef Real CalcType;

    void addControlPoint(const PointType & point);
    void reset();
    bool empty();
    PointType operator()(float t);
    PointType derivative(float t);
    unsigned getLength();

private:
    PointType bezierCurve(const std::vector<PointType> & points, CalcType t);
    CalcType bernsteinPolynomial(unsigned i, unsigned n, CalcType t);
    void calculateDerivativePoints();
    std::vector<PointType> m_points;
    std::vector<PointType> m_derivativePoints;
};

}
}

