#pragma once

#include <gmock/gmock.h>

#include "Common/Game/Attack/IAttack.hpp"

namespace Common
{
namespace Game
{
namespace Attack
{

class AttackMock : public Common::Game::Attack::IAttack
{
public:
    MOCK_METHOD1(attack, void(Common::Game::Object::ObjectBase &));
};

}
}
}
