#pragma once

namespace Client
{
namespace Effects 
{

class IEffect
{
public:
    virtual ~IEffect() {}

    virtual void frameStarted() = 0;
    virtual bool isAlive() = 0;
};

}
}

