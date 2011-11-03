#pragma once

#include <gmock/gmock.h>

#include "Cake/System/ITime.hpp"

namespace Cake 
{
namespace System
{

class TimeMock : public ITime
{
public:
    MOCK_METHOD0(timestamp, unsigned());
};

}
}
