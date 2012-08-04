#pragma once

#include "IAction.hpp"

namespace Server
{
namespace Game
{
namespace Actions
{

class ISimpleAction : public IAction
{
public:
    virtual void execute() = 0;
};

}
}
}
