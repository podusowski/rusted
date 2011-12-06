#include "Common/Logger/Logger.hpp"
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
    LOG_INFO << "New player connected (login: " << message.login << ")\n";

    Common::Messages::UserAuthorizationResp resp;

    try
    {
        Game::Player & player = m_players.create(message.login, message.password, connection);
        LOG_INFO << "Player authorized (id: " << player.getId() << "), registering services\n";
        m_serviceDeployment.deployAuthorizedConnection(connection);
        resp.success = true;
        resp.player_id = player.getId();
    }
    catch (std::exception & ex)
    {
        LOG_WARN << "Couldn't create player, reason: " << ex.what() << "\n";
        resp.success = false;
        resp.player_id = 0;
    }

    connection.send(resp);
}
