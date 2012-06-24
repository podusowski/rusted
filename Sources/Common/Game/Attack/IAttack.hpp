#pragma once

#include "Common/Game/Object/ObjectBase.hpp"

namespace Common
{
namespace Game
{
namespace Attack
{

class IAttack
{
public:
    virtual void attack(Common::Game::Object::ObjectBase &) = 0;
};

}
}
}

