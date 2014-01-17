#include "Cake/Diagnostics/Logger.hpp"

#include "Server/Services/ServiceDeployment.hpp"

using namespace Server::Services;

ServiceDeployment::ServiceDeployment(Cake::Configuration::Configuration & cfg, Server::Game::PlayerContainer & playerContainer) :
    m_rustedTimeService(*m_time),
    m_authorizationService(playerContainer, *this),
    m_playerService(*m_universe, playerContainer),
    m_entityService(*m_universe, playerContainer)
{
}

void ServiceDeployment::deployNewConnection(Server::Network::IConnection & connection)
{
    LOG_DEBUG << "Registering services for new connection";

    connection.addListener(m_serverInfoService);
    connection.addListener(m_authorizationService);
}

void ServiceDeployment::deployAuthorizedConnection(Server::Network::IConnection & connection)
{
    LOG_DEBUG << "Registering services for authorized connection";

    connection.addListener(m_rustedTimeService);
    connection.addListener(m_playerService);
    connection.addListener(m_entityService);
}

void ServiceDeployment::deployAdministrationConnection(Server::Network::IConnection & connection)
{
    LOG_DEBUG << "Registering services for administration connection";

    connection.addListener(m_administrationService);
}
