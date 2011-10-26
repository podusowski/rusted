#pragma once

#include "Common/Messages/Messages.hpp"

#include "Server/Game/PlayerContainer.hpp"
#include "Server/Services/IServiceDeployment.hpp"
#include "Server/Services/AbstractService.hpp"

namespace Server
{
namespace Services 
{

/**
 * @brief Handle non player dedicated signaling.
 */
class AuthorizationService: public Server::AbstractService<AuthorizationService>
{
public:
    AuthorizationService(Game::PlayerContainer &, Services::IServiceDeployment &);
    void handle(const Common::Messages::UserAuthorizationReq &, Network::IConnection &);
    void handle(const Common::Messages::AbstractMessage &, Network::IConnection &) {}

private:
    Game::PlayerContainer & m_players;
    Services::IServiceDeployment & m_serviceDeployment;
};

}

}

