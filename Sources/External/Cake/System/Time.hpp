#pragma once

#include "ITime.hpp"

namespace Cake 
{
namespace System
{

class Time : public ITime
{
public:
    unsigned timestamp();
};

}
}
