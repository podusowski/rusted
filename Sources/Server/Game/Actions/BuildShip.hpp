#pragma once

#include "Server/Game/Actions/IAction.hpp"
#include "Common/Game/Universe.hpp"

namespace Server
{
namespace Game
{
namespace Actions
{

class BuildShip : public IAction
{
public:
    BuildShip(Common::Game::Universe &);
    void execute();

private:
    Common::Game::Universe & m_universe;
};

}
}
}
