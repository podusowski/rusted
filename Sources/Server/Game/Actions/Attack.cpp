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

    sendMovingMeshEffect();

    auto focusedShipPosition = m_focusedShip.getPosition();
    auto selectedShipPosition = m_selectedObject.getPosition();

    auto distance = Common::Game::Position::distance(focusedShipPosition, selectedShipPosition);
    float time = float(distance) / float(m_speed);
    int seconds = floor(time);
    int miliseconds = round((time - seconds) * 100);

    return Common::Game::TimeValue(seconds, miliseconds);
}

Common::Game::TimeValue Attack::finish()
{
    unsigned integrity = m_selectedObject.getIntegrity();
    m_selectedObject.setIntegrity(integrity - 10);

    sendExplosionEffect();
    sendShipInfoToClients();

    return Common::Game::TimeValue(1, 0);
}

void Attack::sendShipInfoToClients()
{
    auto connections = m_playerContainer.getAllConnections(Common::Game::PLAYER_STATE_AUTHORIZED);
    for (auto connection: connections)
    {
        m_servicesUtils.sendObjectIntegrity(m_selectedObject, *connection);
    }
}

void Attack::sendMovingMeshEffect()
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

    auto connections = m_playerContainer.getAllConnections(Common::Game::PLAYER_STATE_AUTHORIZED);
    for (auto connection: connections)
    {
        connection->send(emitMovingMeshEffect);
    }
}

void Attack::sendExplosionEffect()
{
    auto selectedShipPosition = m_selectedObject.getPosition();
    Common::Messages::EmitExplosionEffect emitExplosion;
    emitExplosion.x = selectedShipPosition.getX();
    emitExplosion.y = selectedShipPosition.getY();
    emitExplosion.z = selectedShipPosition.getZ();
    auto connections = m_playerContainer.getAllConnections(Common::Game::PLAYER_STATE_AUTHORIZED);
    for (auto connection: connections)
    {
        connection->send(emitExplosion);
    }
}

