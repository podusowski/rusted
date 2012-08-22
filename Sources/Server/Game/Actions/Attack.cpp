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

    m_selectedObject.setIntegrity(integrity - 10);

    Common::Messages::AttackObject attackObject;
    attackObject.attackerId = m_focusedShip.getId();
    attackObject.attackedId = m_selectedObject.getId();

    auto connections = m_playerContainer.getAllConnections(Server::Game::PLAYER_STATE_AUTHORIZED);
    for (auto connection: connections)
    {
        connection->send(attackObject);
        m_servicesUtils.sendObjectInfo(m_selectedObject, *connection);
    }

    return Common::Game::TimeValue(0, 0);
}

void Attack::finish()
{
}

