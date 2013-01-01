#pragma once

#include "Common/Game/Object/Asteroid.hpp"
#include "Server/Game/Actions/IAction.hpp"

namespace Server
{
namespace Game
{
namespace Actions
{

class Gather : public IAction
{
public:
    Gather(Common::Game::Object::ObjectBase & object);
    Common::Game::TimeValue start();
    void finish();

private:
    Common::Game::Object::Asteroid & m_asteroid;
};

}
}
}

