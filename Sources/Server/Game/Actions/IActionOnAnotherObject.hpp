#pragma once

#include "Common/Game/Object/ObjectBase.hpp"
#include "IAction.hpp"

namespace Server 
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

