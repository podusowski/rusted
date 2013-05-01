#pragma once

#include <vector>

#include "Real.hpp"
#include "Point3.hpp"

namespace Common
{
namespace Math
{

class ISpline3
{
public:
    typedef Point3 PointType;
    typedef Real CalcType;

    virtual void addControlPoint(const PointType &) = 0;
    virtual std::vector<PointType> getControlPoints() const = 0;
    virtual void reset() = 0;
    virtual bool empty() const = 0;
    virtual PointType operator()(float t) const = 0;
    virtual PointType derivative(float t) const = 0;
    virtual unsigned getLength() const = 0;
};

}
}

