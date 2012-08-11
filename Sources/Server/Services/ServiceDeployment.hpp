#pragma once

#include "Cake/DependencyInjection/Inject.hpp"
#include "Cake/Configuration/Configuration.hpp"

#include "Common/Game/RustedTime.hpp"
#include "Server/Network/IConnection.hpp"

#include "Server/Services/IServiceDeployment.hpp"
#include "Server/Services/ServerInfoService.hpp"
#include "Server/Services/RustedTimeService.hpp"
#include "Server/Services/AuthorizationService.hpp"
#include "Server/Services/PlayerService.hpp"
#include "Server/Services/EntityService.hpp"
#include "DataBase/DataBaseFactory.hpp"

#include "Common/Game/Universe.hpp"

namespace Server
{
namespace Services
{

class ServiceDeployment : public IServiceDeployment
{
public:
    ServiceDeployment(Cake::Configuration::Configuration &, Server::DataBase::DataBase &, Server::Game::PlayerContainer &);
    void deployNewConnection(Server::Network::IConnection &);
    void deployAuthorizedConnection(Server::Network::IConnection &);

private:
    Cake::DependencyInjection::Inject<Common::Game::IRustedTime> m_time;
    Server::DataBase::DataBase & m_db;
    Common::Game::Universe m_universe;

    Server::Services::ServerInfoService m_serverInfoService;
    Server::Services::RustedTimeService m_rustedTimeService;
    Server::Services::AuthorizationService m_authorizationService;
    Services::PlayerService m_playerService;
    Services::EntityService m_entityService;
};

}
}
