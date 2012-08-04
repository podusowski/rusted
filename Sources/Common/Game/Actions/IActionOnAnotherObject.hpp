#pragma once

#include "Common/Game/Object/ObjectBase.hpp"
#include "Common/Game/Actions/IAction.hpp"

namespace Common
{
namespace Game
{
namespace Actions
{

class IActionOnAnotherObject : public IAction
{
public:
    virtual void execute(Common::Game::Object::ObjectBase &) = 0;
};

}
}
}

