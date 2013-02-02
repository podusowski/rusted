#include "Gather.hpp"

using namespace Server::Game::Actions;

Gather::Gather(
    Server::Network::IConnection & connection,
    Server::Game::IPlayerContainer & playerContainer, 
    Common::Game::Object::Ship & focusedShip,
    Common::Game::Object::ObjectBase & object)
    : 
    m_connection(connection),
    m_playerContainer(playerContainer),
    m_focusedShip(focusedShip),
    m_asteroid(dynamic_cast<Common::Game::Object::Asteroid&>(object))
{
}

Common::Game::TimeValue Gather::start()
{
    return Common::Game::TimeValue(5, 0);
}

void Gather::finish()
{
    using namespace Common::Game::Object;

    m_asteroid.visitCargoHold([&](CargoHold & asteroidCargoHold) -> void
    {
        m_focusedShip.visitCargoHold([&](CargoHold & shipCargoHold) -> void
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
    m_servicesUtils.sendObjectCargoInfo(m_focusedShip, m_connection);

    // send asteroid cargoinfo
    auto connections = m_playerContainer.getAllConnections(Common::Game::PLAYER_STATE_AUTHORIZED);
    for (auto connection: connections)
    {
        m_servicesUtils.sendObjectCargoInfo(m_asteroid, *connection);
    }
}

