#pragma once

#include <boost/function.hpp>

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
    virtual void createTimer(TimeValue, boost::function<void()>) = 0;

    virtual ~IRustedTime() {}
};

}
}

