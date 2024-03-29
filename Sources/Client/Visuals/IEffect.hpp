#pragma once

namespace Client
{
namespace Visuals
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

