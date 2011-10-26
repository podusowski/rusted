#pragma once

namespace Client
{
namespace States
{

class IStateManagerStack
{
public:
    virtual void pushState(IState &) = 0;
    virtual IState & popState() = 0;
};

}
}

