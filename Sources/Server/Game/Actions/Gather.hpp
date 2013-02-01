#pragma once

#include "Server/Game/IPlayerContainer.hpp"
#include "Common/Game/Object/Asteroid.hpp"
#include "Common/Game/Object/Ship.hpp"
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
    Gather(Server::Network::IConnection &, IPlayerContainer &, Common::Game::Object::Ship &, Common::Game::Object::ObjectBase & object);
    Common::Game::TimeValue start();
    void finish();

private:
    void sendCargoInfoToClients();

    Server::Network::IConnection & m_connection;
    IPlayerContainer & m_playerContainer;
    Common::Game::Object::Ship & m_focusedShip;
    Common::Game::Object::Asteroid & m_asteroid;
    Server::Services::Utils m_servicesUtils;
};

}
}
}

