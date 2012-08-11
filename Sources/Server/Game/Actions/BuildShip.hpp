#pragma once

#include "Server/Game/Actions/IAction.hpp"
#include "Common/Game/Universe.hpp"
#include "Server/Game/IPlayer.hpp"

namespace Server
{
namespace Game
{
namespace Actions
{

class BuildShip : public IAction
{
public:
    BuildShip(Common::Game::Universe &, Server::Game::IPlayer &);
    void execute();

private:
    Common::Game::Universe & m_universe;
    Server::Game::IPlayer & m_player;
};

}
}
}
