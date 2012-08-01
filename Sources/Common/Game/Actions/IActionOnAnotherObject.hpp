#pragma once

#include "Common/Game/Object/ObjectBase.hpp"

namespace Common
{
namespace Game
{
namespace Actions
{

class IActionOnAnotherObject
{
public:
    virtual void execute(Common::Game::Object::ObjectBase &) = 0;
};

}
}
}

