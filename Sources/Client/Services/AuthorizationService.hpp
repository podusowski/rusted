#pragma once

#include <string>
#include <boost/function.hpp>

#include "Common/Game/Player.hpp"
#include "Network/IConnection.hpp"
#include "Client/Services/AbstractService.hpp"

namespace Client
{
namespace Services
{

class AuthorizationService : public AbstractService<AuthorizationService> 
{
public:
    typedef boost::function<void(bool)> LoggedInCallback;

    AuthorizationService(Network::IConnection &, Common::Game::Player &);
    void login(std::string login, std::string password, LoggedInCallback);

    void handle(const Common::Messages::ServerVersionResp &);
    void handle(const Common::Messages::UserAuthorizationResp &);
    void handle(const Common::Messages::Exception &);

    void handle(const Common::Messages::AbstractMessage &) {}

private:
    LoggedInCallback m_loggedInCallback;
    bool m_versionChecked;
    Network::IConnection & m_connection;
    std::string m_login;
    std::string m_password;
    Common::Game::Player & m_player;
};

}
}

