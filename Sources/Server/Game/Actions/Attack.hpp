#pragma once

#include "Server/Game/Actions/IActionOnAnotherObject.hpp"

namespace Server
{
namespace Game
{
namespace Actions
{

class Attack : public IActionOnAnotherObject
{
public:
    void execute(Common::Game::Object::ObjectBase & object);
};

}
}
}

