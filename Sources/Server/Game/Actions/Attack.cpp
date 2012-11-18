#include "Cake/Diagnostics/Logger.hpp"
#include "Attack.hpp"

using namespace Server::Game::Actions;

Attack::Attack(IPlayerContainer & playerContainer, Common::Game::Object::Ship & focusedShip, Common::Game::Object::ObjectBase & selectedObject) :
    m_playerContainer(playerContainer),
    m_focusedShip(focusedShip),
    m_selectedObject(selectedObject)
{
}

Common::Game::TimeValue Attack::start()
{
    unsigned integrity = m_selectedObject.getIntegrity();

    if (integrity == 0)
    {
        LOG_DEBUG << "Tried to attack destroyed object";
        return Common::Game::TimeValue(0, 0);
    }

    auto focusedShipPosition = m_focusedShip.getPosition();
    auto selectedShipPosition = m_selectedObject.getPosition();

    Common::Messages::EmitMovingMeshEffect emitMovingMeshEffect;
    emitMovingMeshEffect.fromX = focusedShipPosition.getX();
    emitMovingMeshEffect.fromY = focusedShipPosition.getY();
    emitMovingMeshEffect.fromZ = focusedShipPosition.getZ();
    emitMovingMeshEffect.toX = selectedShipPosition.getX();
    emitMovingMeshEffect.toY = selectedShipPosition.getY();
    emitMovingMeshEffect.toZ = selectedShipPosition.getZ();
    emitMovingMeshEffect.speed = 1000;

    m_selectedObject.setIntegrity(integrity - 10);

    Common::Messages::AttackObject attackObject;
    attackObject.attackerId = m_focusedShip.getId();
    attackObject.attackedId = m_selectedObject.getId();

    auto connections = m_playerContainer.getAllConnections(Server::Game::PLAYER_STATE_AUTHORIZED);
    for (auto connection: connections)
    {
        connection->send(emitMovingMeshEffect);
        connection->send(attackObject);
        m_servicesUtils.sendObjectInfo(m_selectedObject, *connection);
    }

    return Common::Game::TimeValue(0, 0);
}

void Attack::finish()
{
}

