#pragma once

#include <gmock/gmock.h>

#include "Common/Math/ISpline3.hpp"

namespace Common
{
namespace Math
{

class Spline3Mock : public ISpline3
{
public:
    MOCK_METHOD1(addControlPoint, void(const PointType &));
    MOCK_CONST_METHOD0(getControlPoints, std::vector<PointType>());
    MOCK_METHOD0(reset, void());
    MOCK_CONST_METHOD0(empty, bool());
    MOCK_CONST_METHOD1(value, PointType(float t));
    MOCK_CONST_METHOD1(derivative, PointType(float t));
    MOCK_CONST_METHOD0(getLength, unsigned());
};

}
}

