#include "Cake/Diagnostics/Logger.hpp"
#include "Common/Messages/Messages.hpp"
#include "Services/AuthorizationService.hpp"

using namespace Server::Services;

AuthorizationService::AuthorizationService(Game::PlayerContainer & players, Services::IServiceDeployment & serviceDeployment) :
    m_players(players),
    m_serviceDeployment(serviceDeployment)
{
}

void AuthorizationService::handle(const Common::Messages::UserAuthorizationReq & message, Network::IConnection & connection)
{
    LOG_DEBUG << "Player is trying to authorize (login: " << message.login << ")";

    Common::Messages::UserAuthorizationResp resp;

    try
    {
        int id = m_players.authorize(message.login, message.password, connection);
        LOG_DEBUG << "Player authorized (id: " << id << "), registering services";
        m_serviceDeployment.deployAuthorizedConnection(connection);
        resp.success = true;
        resp.player_id = id;
    }
    catch (std::exception & ex)
    {
        LOG_WARN << "Couldn't authorize player, reason: " << ex.what();
        resp.success = false;
        resp.player_id = 0;
    }

    connection.send(resp);
}
