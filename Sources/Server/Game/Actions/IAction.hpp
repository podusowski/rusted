#pragma once

namespace Server 
{
namespace Game
{
namespace Actions
{

class IAction
{
public:
    virtual void execute() = 0;

    virtual ~IAction() {}
};

}
}
}
