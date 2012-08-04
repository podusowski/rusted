#include "Attack.hpp"

using namespace Server::Game::Actions;

Attack::Attack(IPlayerContainer & playerContainer, Common::Game::Object::Ship & focusedShip, Common::Game::Object::ObjectBase & selectedObject) :
    m_playerContainer(playerContainer),
    m_focusedShip(focusedShip),
    m_selectedObject(selectedObject)
{
}

void Attack::execute()
{
    unsigned integrity = m_selectedObject.getIntegrity();
    m_selectedObject.setIntegrity(integrity - 10);

    Common::Messages::AttackObject attackObject;
    attackObject.attackerId = m_focusedShip.getId();
    attackObject.attackedId = m_selectedObject.getId();

    auto connections = m_playerContainer.getAllConnections(Server::Game::PLAYER_STATE_AUTHORIZED);
    for (auto connection: connections)
    {
        connection->send(attackObject);

        // FIXME: this might not be Ship - create and use some API to sending objectInfo
        sendShipInfo(dynamic_cast<Common::Game::Object::Ship&>(m_selectedObject), *connection);
    }
}

// FIXME: duplicated in EntityService
void Attack::sendShipInfo(Common::Game::Object::Ship & ship, Network::IConnection & connection)
{
    Common::Messages::ShipInfo shipInfo;

    Common::Game::Position position = ship.getPosition();
    shipInfo.id = ship.getId();
    shipInfo.player_id = ship.getOwnerId();
    shipInfo.x = position.getX();
    shipInfo.y = position.getY();
    shipInfo.z = position.getZ();
    shipInfo.integrity = ship.getIntegrity();

    connection.send(shipInfo);
}

