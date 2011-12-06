#include "Common/Logger/Logger.hpp"
#include "Services/AuthorizationService.hpp"

using namespace Client::Services;

AuthorizationService::AuthorizationService(Network::Connection & connection, Game::PlayerInfo & playerInfo) : 
    m_versionChecked(false),
    m_connection(connection),
    m_playerInfo(playerInfo)
{
}

void AuthorizationService::login(std::string login, std::string password, LoggedInCallback callback)
{
    m_loggedInCallback = callback;
    m_login = login;
    m_password = password;
    Common::Messages::ServerVersionReq versionReq;
    m_connection.send(versionReq);
}

void AuthorizationService::handle(const Common::Messages::ServerVersionResp & serverVersionResp)
{
    LOG_INFO << "Server reported version: " << serverVersionResp.version << "\n";

    if (!m_versionChecked)
    {
        m_versionChecked = true;
        Common::Messages::UserAuthorizationReq req;
        req.login = m_login;
        req.password = m_password;
        m_connection.send(req);
    }
}

void AuthorizationService::handle(const Common::Messages::UserAuthorizationResp & userAuthorizationResp)
{
    LOG_INFO << "Received user authorization response (player id: " << userAuthorizationResp.player_id << "\n";
    m_playerInfo.setId(userAuthorizationResp.player_id);
    m_loggedInCallback(userAuthorizationResp.success);
}
