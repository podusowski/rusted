#include "Transfer.hpp"

using namespace Server::Game::Actions;

Transfer::Transfer(
    Server::Network::IConnection & connection,
    IPlayerContainer & playerContainer,
    Common::Game::IPlayer & player)
    :
    m_connection(connection),
    m_playerContainer(playerContainer),
    m_player(player)
{
}

Common::Game::TimeValue Transfer::start()
{
    using namespace Common::Game::Object;

    auto & selectedShip = dynamic_cast<Common::Game::Object::Ship &>(m_player.getSelectedObject());
    auto & focusedShip = dynamic_cast<Common::Game::Object::Ship &>(m_player.getFocusedObject());

    //TODO: additional parameters
    selectedShip.visitCargoHold([&](CargoHold & selectedShipCargoHold) -> void
    {
        focusedShip.visitCargoHold([&](CargoHold & focusedShipCargoHold) -> void
        {
            focusedShipCargoHold.changeCarbon(-10);
            focusedShipCargoHold.changeHelium(-10);

            selectedShipCargoHold.changeCarbon(10);
            selectedShipCargoHold.changeHelium(10);
        });
    });

    m_servicesUtils.sendObjectCargoInfo(focusedShip, m_connection);

    auto & otherPlayerConnection = m_playerContainer.getConnectionById(selectedShip.getOwnerId());
    m_servicesUtils.sendObjectCargoInfo(selectedShip, otherPlayerConnection);

    return Common::Game::TimeValue();
}

Common::Game::TimeValue Transfer::finish()
{
    return Common::Game::TimeValue();
}

bool Transfer::isAbleToStart()
{
    return true;
}

