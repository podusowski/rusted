#pragma once

#include "Common/Game/Object/ObjectBase.hpp"

namespace Common
{
namespace Game
{
namespace Attack
{

class Attack
{
public:
    void operator()(Common::Game::Object::ObjectBase &);
};

}
}
}
