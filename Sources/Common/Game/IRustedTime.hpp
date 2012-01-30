#pragma once

#include "TimeValue.hpp"

namespace Common 
{
namespace Game 
{

class IRustedTime
{
public:
    virtual unsigned getSeconds() = 0;
    virtual TimeValue getCurrentTime() = 0;
    virtual void setReferenceTime(unsigned) = 0;

    virtual ~IRustedTime() {}
};

}
}

