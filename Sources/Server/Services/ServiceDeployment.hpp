#pragma once

#include "Cake/DependencyInjection/Inject.hpp"
#include "Cake/Configuration/Configuration.hpp"

#include "Common/Game/RustedTime.hpp"
#include "Server/Network/IConnection.hpp"

#include "Game/ShipClassContainer.hpp"

#include "Server/Services/IServiceDeployment.hpp"
#include "Server/Services/ServerInfoService.hpp"
#include "Server/Services/RustedTimeService.hpp"
#include "Server/Services/AuthorizationService.hpp"
#include "Server/Services/PlayerService.hpp"
#include "Server/Services/EntityService.hpp"
#include "Server/Services/AdministrationService.hpp"

#include "Common/Game/Universe.hpp"

namespace Server
{
namespace Services
{

class ServiceDeployment : public IServiceDeployment
{
public:
    ServiceDeployment(Cake::Configuration::Configuration &, Server::Game::PlayerContainer &);
    void deployNewConnection(Server::Network::IConnection &);
    void deployAuthorizedConnection(Server::Network::IConnection &);
    void deployAdministrationConnection(Server::Network::IConnection &);

private:
    Cake::DependencyInjection::Inject<Common::Game::IRustedTime> m_time;
    Cake::DependencyInjection::Inject<Common::Game::Universe> m_universe;

    Services::ServerInfoService m_serverInfoService;
    Services::RustedTimeService m_rustedTimeService;
    Services::AuthorizationService m_authorizationService;
    Services::PlayerService m_playerService;
    Services::EntityService m_entityService;
    Services::AdministrationService m_administrationService;
};

}
}
