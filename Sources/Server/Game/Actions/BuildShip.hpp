#pragma once

#include "Common/Game/Universe.hpp"
#include "Common/Game/IPlayer.hpp"

#include "Server/Game/Actions/IAction.hpp"
#include "Server/Game/IPlayerContainer.hpp"
#include "Server/Game/IUniverseDataBaseFacade.hpp"
#include "Server/Game/IShipClassContainer.hpp"
#include "Server/Services/Utils.hpp"

namespace Server
{
namespace Game
{
namespace Actions
{

// FIXME: FocusedObject will be invalid if player select other one during action time
class BuildShip : public IAction
{
public:
    BuildShip(Common::Game::Universe &, Common::Game::IPlayer &, Server::Game::IPlayerContainer &, unsigned shipClass);
    Common::Game::TimeValue start();
    Common::Game::TimeValue finish();
    bool isAbleToStart();
    std::string getName() const;
    std::string getDescription() const;

private:
    void updateCargoHold();

    Common::Game::Universe & m_universe;
    Common::Game::IPlayer & m_player;
    Server::Game::IPlayerContainer & m_playerContainer;
    unsigned m_shipClass;
    Server::Services::Utils m_servicesUtils;
    Cake::DependencyInjection::Inject<IUniverseDataBaseFacade> m_universeDataBaseFacade;
    Cake::DependencyInjection::Inject<IShipClassContainer> m_shipClassContainer;
};

}
}
}
