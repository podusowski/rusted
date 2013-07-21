#pragma once

#include "Common/Game/Object/Asteroid.hpp"
#include "Common/Game/Object/Ship.hpp"
#include "Common/Game/IPlayer.hpp"

#include "Server/Game/IPlayerContainer.hpp"
#include "Server/Game/Actions/IAction.hpp"
#include "Server/Services/Utils.hpp"

namespace Server
{
namespace Game
{
namespace Actions
{

class Gather : public IAction
{
public:
    Gather(Server::Network::IConnection &, IPlayerContainer &, Common::Game::IPlayer &);
    Common::Game::TimeValue start();
    Common::Game::TimeValue finish();
    bool isAbleToStart();

private:
    void sendCargoInfoToClients();

    Server::Network::IConnection & m_connection;
    IPlayerContainer & m_playerContainer;
    Common::Game::IPlayer & m_player;
    Server::Services::Utils m_servicesUtils;
};

}
}
}

