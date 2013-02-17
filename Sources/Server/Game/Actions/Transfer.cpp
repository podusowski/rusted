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
        focusedShip.visitCargoHold([&](CargoHold & shipCargoHold) -> void
        {
            selectedShipCargoHold.changeCarbon(-10);
            selectedShipCargoHold.changeHelium(-10);

            shipCargoHold.changeCarbon(10);
            shipCargoHold.changeHelium(10);
        });
    });

//    sendCargoInfoToClients();
}

void Transfer::finish()
{
}

