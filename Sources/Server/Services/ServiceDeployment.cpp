#include "Cake/Diagnostics/Logger.hpp"

#include "Server/Services/ServiceDeployment.hpp"
#include "Common/Game/UniverseLoader.hpp"

using namespace Server::Services;

ServiceDeployment::ServiceDeployment(Common::Configuration::Configuration & cfg) :
    m_dbFactory(cfg),
    m_db(m_dbFactory.create()),

    m_players(m_db),

    m_rustedTimeService(*m_time),
    m_authorizationService(m_players, *this),
    m_playerService(m_universe),
    m_entityService(m_universe),
    m_staticObjectsService(m_universe)
{
    Common::Game::UniverseLoader loader;
    loader.load(m_universe, m_db);
}

void ServiceDeployment::deployNewConnection(Server::Network::IConnection & connection)
{
    LOG_INFO << "Registering services for new connection";

    connection.addListener(m_serverInfoService);
    connection.addListener(m_authorizationService);
}

void ServiceDeployment::deployAuthorizedConnection(Server::Network::IConnection & connection)
{
    LOG_INFO << "Registering services for authorized connection";

    connection.addListener(m_rustedTimeService);
    connection.addListener(m_playerService);
    connection.addListener(m_entityService);
    connection.addListener(m_staticObjectsService);
}
