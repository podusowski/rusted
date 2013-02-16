#include "Gather.hpp"

using namespace Server::Game::Actions;

Gather::Gather(
    Server::Network::IConnection & connection,
    Server::Game::IPlayerContainer & playerContainer, 
    Common::Game::IPlayer & player)
    : 
    m_connection(connection),
    m_playerContainer(playerContainer),
    m_player(player)
{
}

Common::Game::TimeValue Gather::start()
{
    return Common::Game::TimeValue(5, 0);
}

void Gather::finish()
{
    using namespace Common::Game::Object;
    
    auto & asteroid = dynamic_cast<Common::Game::Object::Asteroid &>(m_player.getSelectedObject());
    auto & focusedShip = dynamic_cast<Common::Game::Object::Ship &>(m_player.getFocusedObject());

    asteroid.visitCargoHold([&](CargoHold & asteroidCargoHold) -> void
    {
        focusedShip.visitCargoHold([&](CargoHold & shipCargoHold) -> void
        {
            asteroidCargoHold.changeCarbon(-10);
            asteroidCargoHold.changeHelium(-10);

            shipCargoHold.changeCarbon(10);
            shipCargoHold.changeHelium(10);
        });
    });

    sendCargoInfoToClients();
}

void Gather::sendCargoInfoToClients()
{
    // send cargoinfo of player's ship
    auto & focusedShip = dynamic_cast<Common::Game::Object::Ship &>(m_player.getFocusedObject());
    m_servicesUtils.sendObjectCargoInfo(focusedShip, m_connection);

    // send asteroid cargoinfo
    auto & asteroid = dynamic_cast<Common::Game::Object::Asteroid &>(m_player.getSelectedObject());
    auto connections = m_playerContainer.getAllConnections(Common::Game::PLAYER_STATE_AUTHORIZED);
    for (auto connection: connections)
    {
        m_servicesUtils.sendObjectCargoInfo(asteroid, *connection);
    }
}

