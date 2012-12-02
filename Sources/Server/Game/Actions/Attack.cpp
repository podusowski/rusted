#include "Cake/Diagnostics/Logger.hpp"
#include "Attack.hpp"

using namespace Server::Game::Actions;

Attack::Attack(IPlayerContainer & playerContainer, Common::Game::Object::Ship & focusedShip, Common::Game::Object::ObjectBase & selectedObject) :
    m_playerContainer(playerContainer),
    m_focusedShip(focusedShip),
    m_selectedObject(selectedObject),
    m_speed(1000)
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

    sendEffect();

    auto focusedShipPosition = m_focusedShip.getPosition();
    auto selectedShipPosition = m_selectedObject.getPosition();

    auto distance = Common::Game::Position::distance(focusedShipPosition, selectedShipPosition);
    int seconds = floor(float(distance) / float(m_speed));

    return Common::Game::TimeValue(seconds, 0);
}

void Attack::finish()
{
    unsigned integrity = m_selectedObject.getIntegrity();
    m_selectedObject.setIntegrity(integrity - 10);

    sendShipInfoToClients();
}

void Attack::sendShipInfoToClients()
{
    auto connections = m_playerContainer.getAllConnections(Server::Game::PLAYER_STATE_AUTHORIZED);
    for (auto connection: connections)
    {
        m_servicesUtils.sendObjectInfo(m_selectedObject, *connection);
    }
}

void Attack::sendEffect()
{
    auto focusedShipPosition = m_focusedShip.getPosition();
    auto selectedShipPosition = m_selectedObject.getPosition();

    Common::Messages::EmitMovingMeshEffect emitMovingMeshEffect;
    emitMovingMeshEffect.fromX = focusedShipPosition.getX();
    emitMovingMeshEffect.fromY = focusedShipPosition.getY();
    emitMovingMeshEffect.fromZ = focusedShipPosition.getZ();
    emitMovingMeshEffect.toX = selectedShipPosition.getX();
    emitMovingMeshEffect.toY = selectedShipPosition.getY();
    emitMovingMeshEffect.toZ = selectedShipPosition.getZ();
    emitMovingMeshEffect.speed = m_speed;

    Common::Messages::AttackObject attackObject;
    attackObject.attackerId = m_focusedShip.getId();
    attackObject.attackedId = m_selectedObject.getId();

    auto connections = m_playerContainer.getAllConnections(Server::Game::PLAYER_STATE_AUTHORIZED);
    for (auto connection: connections)
    {
        connection->send(emitMovingMeshEffect);
        connection->send(attackObject);
    }
}

