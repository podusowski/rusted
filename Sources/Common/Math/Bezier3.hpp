#pragma once

#include "Point3.hpp"
#include <vector>
#include <boost/math/special_functions/binomial.hpp>
#include <cmath>

namespace Common
{
namespace Math
{

class Bezier3
{
public:
    typedef Point3<int> PointType;
    typedef float CalcType;

    void addControlPoint(const PointType & point);
    void reset();
    bool empty();
    PointType operator()(float t);
    unsigned getLength();

private:
    CalcType bernsteinPolynomial(unsigned i, unsigned n, CalcType t);
    std::vector<PointType> m_points;
};

}
}

