#pragma once

#include "Common/Game/Object/ObjectBase.hpp"
#include "Common/Game/Attack/IAttack.hpp"

namespace Common
{
namespace Game
{
namespace Attack
{

class Attack : public IAttack
{
public:
    void attack(Common::Game::Object::ObjectBase &);
};

}
}
}
