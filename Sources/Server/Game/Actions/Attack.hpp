#pragma once

#include "Common/Game/Actions/IActionOnAnotherObject.hpp"

namespace Server
{
namespace Game
{
namespace Actions
{

class Attack
{
public:
    void execute(Common::Game::Object::ObjectBase & object)
    {
        unsigned integrity = object.getIntegrity();
        object.setIntegrity(integrity - 10);
    }
};

}
}
}

