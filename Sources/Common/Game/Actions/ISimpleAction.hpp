#pragma once

#include "Common/Game/Actions/IAction.hpp"

namespace Common
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
