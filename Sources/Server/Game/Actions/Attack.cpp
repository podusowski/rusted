#include "Cake/Diagnostics/Logger.hpp"
#include "Attack.hpp"

using namespace Server::Game::Actions;

Attack::Attack(Common::Game::Universe & universe, IPlayerContainer & playerContainer, const ActionParameters & actionParameters) :
    m_universe(universe),
    m_playerContainer(playerContainer),
    m_actionParameters(actionParameters),
    m_speed(1000)
{
}

Common::Game::TimeValue Attack::start()
{
    auto & focusedObject = m_universe.getById<Common::Game::Object::Ship>(m_actionParameters.focusedObjectId.get());
    auto & selectedObject = m_universe.getById<Common::Game::Object::Ship>(m_actionParameters.selectedObjectId.get());

    unsigned integrity = selectedObject.getIntegrity();

    if (integrity == 0)
    {
        LOG_DEBUG << "Tried to attack destroyed object";
        return Common::Game::TimeValue(0, 0);
    }

    if (selectedObject.getOwnerId() == m_actionParameters.playerId.get())
    {
        LOG_DEBUG << "Tried to attack own ship";
        return Common::Game::TimeValue(0, 0);
    }

    sendMovingMeshEffect();

    auto focusedShipPosition = focusedObject.getPosition();
    auto selectedShipPosition = selectedObject.getPosition();

    auto distance = Common::Game::Position::distance(focusedShipPosition, selectedShipPosition);
    float time = float(distance) / float(m_speed);
    int seconds = floor(time);
    int miliseconds = round((time - seconds) * 100);

    return Common::Game::TimeValue(seconds, miliseconds);
}

Common::Game::TimeValue Attack::finish()
{
    auto & selectedObject = m_universe.getById<Common::Game::Object::Ship>(m_actionParameters.selectedObjectId.get());

    if (selectedObject.getOwnerId() == m_actionParameters.playerId.get())
    {
        LOG_DEBUG << "Tried to attack own ship";
        return Common::Game::TimeValue(0, 0);
    }

    unsigned integrity = selectedObject.getIntegrity();

    if (integrity > 0)
    {
        selectedObject.setIntegrity(integrity - 10);
    }
    else
    {
        LOG_DEBUG << "Tried to attack destroyed object";
    }

    sendExplosionEffect();
    sendShipInfoToClients();

    return Common::Game::TimeValue(1, 0);
}

bool Attack::isAbleToStart()
{
    auto & selectedObject = m_universe.getById<Common::Game::Object::Ship>(m_actionParameters.selectedObjectId.get());

    if (selectedObject.getOwnerId() == m_actionParameters.playerId.get())
    {
        LOG_DEBUG << "Tried to attack own ship";
        return false;
    }

    unsigned integrity = selectedObject.getIntegrity();

    if (integrity > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Attack::sendShipInfoToClients()
{
    auto & selectedObject = m_universe.getById<Common::Game::Object::Ship>(m_actionParameters.selectedObjectId.get());

    auto connections = m_playerContainer.getAllConnections(Common::Game::PLAYER_STATE_AUTHORIZED);
    for (auto connection: connections)
    {
        m_servicesUtils.sendObjectIntegrity(selectedObject, *connection);
    }
}

void Attack::sendMovingMeshEffect()
{
    auto & focusedObject = m_universe.getById<Common::Game::Object::Ship>(m_actionParameters.focusedObjectId.get());
    auto & selectedObject = m_universe.getById<Common::Game::Object::Ship>(m_actionParameters.selectedObjectId.get());

    auto focusedShipPosition = focusedObject.getPosition();
    auto selectedShipPosition = selectedObject.getPosition();

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
    auto & selectedObject = m_universe.getById<Common::Game::Object::Ship>(m_actionParameters.selectedObjectId.get());
    auto & focusedObject = m_universe.getById<Common::Game::Object::Ship>(m_actionParameters.focusedObjectId.get());
    auto focusedObjectPosition = focusedObject.getPosition();
    auto selectedObjectPosition = selectedObject.getPosition();

    auto direction = selectedObjectPosition - focusedObjectPosition;

    Common::Messages::EmitExplosionEffect emitExplosion;
    emitExplosion.objectId = m_actionParameters.selectedObjectId.get();
    emitExplosion.directionX = direction.getX();
    emitExplosion.directionY = direction.getY();
    emitExplosion.directionZ = direction.getZ();

    auto connections = m_playerContainer.getAllConnections(Common::Game::PLAYER_STATE_AUTHORIZED);
    for (auto connection: connections)
    {
        connection->send(emitExplosion);
    }
}

