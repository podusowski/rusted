#pragma once

namespace Client
{
namespace States
{

class IState
{
public:
    virtual void activate() = 0;
    virtual void deactivate() = 0;
    virtual void frameStarted() = 0;

    virtual ~IState() {}
};

}
}
