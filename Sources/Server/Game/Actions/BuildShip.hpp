#pragma once

#include "Common/Game/Universe.hpp"
#include "Common/Game/IPlayer.hpp"

#include "Server/Game/Actions/IAction.hpp"
#include "Server/Game/IPlayerContainer.hpp"
#include "Server/Game/IObjectFactory.hpp"
#include "Server/Services/Utils.hpp"

namespace Server
{
namespace Game
{
namespace Actions
{

class BuildShip : public IAction
{
public:
    BuildShip(Common::Game::Universe &, Common::Game::IPlayer &, Server::Game::IPlayerContainer &, unsigned shipClass);
    Common::Game::TimeValue start();
    void finish();

private:
    Common::Game::Universe & m_universe;
    Common::Game::IPlayer & m_player;
    Server::Game::IPlayerContainer & m_playerContainer;
    unsigned m_shipClass;
    Server::Services::Utils m_servicesUtils;
    Cake::DependencyInjection::Inject<IObjectFactory> m_objectFactory;
};

}
}
}
