#pragma once

#include "Common/Game/Object/Ship.hpp"
#include "Common/Game/IPlayer.hpp"

#include "Server/Game/Actions/IAction.hpp"
#include "Server/Network/IConnection.hpp"
#include "Server/Game/IPlayerContainer.hpp"
#include "Server/Services/Utils.hpp"

namespace Server
{
namespace Game
{
namespace Actions
{

class Transfer : public IAction
{
public:
    Transfer(Server::Network::IConnection &, IPlayerContainer &, Common::Game::IPlayer &);
    Common::Game::TimeValue start();
    Common::Game::TimeValue finish();
    bool isAbleToStart();
    std::string getName() const;
    std::string getDescription() const;

private:
    Server::Network::IConnection & m_connection;
    IPlayerContainer & m_playerContainer;
    Common::Game::IPlayer & m_player;
    Server::Services::Utils m_servicesUtils;
};

}
}
}
