#pragma once

#include "Common/Game/Actions/IActionOnAnotherObject.hpp"

namespace Server
{
namespace Game
{
namespace Actions
{

class Attack : public Common::Game::Actions::IActionOnAnotherObject
{
public:
    void execute(Common::Game::Object::ObjectBase & object);
};

}
}
}

