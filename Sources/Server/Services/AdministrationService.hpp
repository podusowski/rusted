#pragma once

#include "Cake/DependencyInjection/Inject.hpp"

#include "Common/Game/Universe.hpp"

#include "Game/IUniverseDataBaseFacade.hpp"
#include "Game/IPlayerContainer.hpp"
#include "Services/AbstractService.hpp"
#include "Utils.hpp"

namespace Server
{
namespace Services
{

class AdministrationService : public Server::AbstractService<AdministrationService>
{
public:
    AdministrationService(Game::IPlayerContainer &);

    void handle(const Common::Messages::ReloadDatabase &, Network::IConnection &);

    void handle(const Common::Messages::AbstractMessage &, Network::IConnection &) {}

private:
    Cake::DependencyInjection::Inject<Game::IUniverseDataBaseFacade> m_universeDataBaseFacade;
    Cake::DependencyInjection::Inject<Common::Game::Universe> m_universe;
    Game::IPlayerContainer & m_playerContainer;
    Utils m_utils;
};

}
}

