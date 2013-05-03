#pragma once

#include <vector>

#include "Real.hpp"
#include "Point3.hpp"
#include "Quaternion.hpp"
#include "ISpline3.hpp"

namespace Common
{
namespace Math
{

class Bezier3 : public ISpline3
{
public:
    void addControlPoint(const PointType & point);
    std::vector<PointType> getControlPoints() const;
    void reset();
    bool empty() const;
    PointType value(float t) const;
    PointType derivative(float t) const;
    unsigned getLength() const;

private:
    PointType bezierCurve(const std::vector<PointType> & points, CalcType t) const;
    CalcType bernsteinPolynomial(unsigned i, unsigned n, CalcType t) const;
    void calculateDerivativePoints();
    std::vector<PointType> m_points;
    std::vector<PointType> m_derivativePoints;
};

}
}

