#include "Common/Game/Object/Ship.hpp"

#include "Game/Actions/ActionType.hpp"
#include "Server/Services/EntityService.hpp"

using namespace Server::Services;

EntityService::EntityService(
    Common::Game::Universe & universe,
    Server::Game::PlayerContainer & playerContainer
) :
    m_universe(universe),
    m_playerContainer(playerContainer),
    m_actionFactory(universe, playerContainer),
    m_actionPerformer(m_actionFactory, m_universe, playerContainer)
{
}

void EntityService::handle(const Common::Messages::ChangeShipCourse & changeCourseReq, Network::IConnection & connection)
{
    Common::Game::Object::Ship & ship = m_universe.getById<Common::Game::Object::Ship>(changeCourseReq.shipId);

    Common::Game::Position destination(changeCourseReq.x,
                                       changeCourseReq.y,
                                       changeCourseReq.z);
    ship.setCourse(destination);

    std::vector<Network::IConnection *> connections = m_playerContainer.getAllConnections(Common::Game::PLAYER_STATE_AUTHORIZED);
    for (std::vector<Network::IConnection *>::iterator it = connections.begin(); it != connections.end(); it++)
    {
        if (*it != &connection)
        {
            m_utils.sendShipCourseInfo(ship, **it);
        }
    }
}

void EntityService::handle(const Common::Messages::GetVisibleObjects &, Network::IConnection & connection)
{
    Common::Messages::VisibleObjects visibleObjects;

    LOG_DEBUG << "Filling visible objects";

    auto objects = m_universe.getAll(); 
    for (auto object: objects)
    {
        LOG_DEBUG << "  id: " << object->getId() << ", type: " << TYPENAME(*object);

        Common::Messages::ObjectId objectId;
        objectId.id = object->getId();

        visibleObjects.objects.push_back(objectId);
    }

    connection.send(visibleObjects);
}

void EntityService::handle(const Common::Messages::GetObjectInfo & getObjectInfo, Network::IConnection & connection)
{
    m_utils.sendObjectInfo(getObjectInfo.id, m_universe, connection);
}

void EntityService::handle(const Common::Messages::SelectObject & selectObject, Network::IConnection & connection)
{
    auto & player = m_playerContainer.getBy(connection);
    auto & object = m_universe.getById<Common::Game::Object::ObjectBase>(selectObject.id);

    LOG_DEBUG << "Player " << player.getId() << " is selecting " << object;

    player.selectObject(object);
}

void EntityService::handle(const Common::Messages::FocusObject & focusObject, Network::IConnection & connection)
{
    auto & player = m_playerContainer.getBy(connection);
    auto & object = m_universe.getById<Common::Game::Object::ObjectBase>(focusObject.id);

    LOG_DEBUG << "Player " << player.getId() << " is focusing " << object;

    player.focusObject(object);
}

void EntityService::handle(const Common::Messages::FetchAvailableActions &, Network::IConnection & connection)
{
    auto & player = m_playerContainer.getBy(connection);
    auto & object = dynamic_cast<Common::Game::Object::Ship&>(player.getFocusedObject());
    auto & shipClass = m_shipClassContainer->getById(object.getClass());
    auto actions = shipClass.getAvailableActions();

    Common::Messages::AvailableActions availableActions;
    LOG_DEBUG << "Filling actions for ship:" << object.getId() << " (class:" << shipClass.getId() << ")";
    for (auto action: actions)
    {
        std::string name = m_actionFactory.getActionName(action.type, action.parameter);
        std::string description = m_actionFactory.getActionDescription(action.type, action.parameter);

        LOG_DEBUG << "  name:" << name << ", type:" << action.type << ", parameter:" << action.parameter;

        Common::Messages::AvailableAction availableAction;
        availableAction.id = action.type;
        availableAction.name = name;
        availableAction.parameter = action.parameter;
        availableAction.description = description;

        availableActions.actions.push_back(availableAction);
    }
    connection.send(availableActions);
}

void EntityService::handle(const Common::Messages::ExecuteAction & executeAction, Network::IConnection & connection)
{
    auto & player = m_playerContainer.getBy(connection);

    LOG_DEBUG << "Player " << player.getId() << " is executing action id:" 
              << executeAction.id << ", parameter:" << executeAction.parameter;

    try
    {
        m_actionPerformer.perform(connection, player, executeAction.id, executeAction.parameter);
    }
    catch (const std::exception & ex)
    {
        LOG_DEBUG << "Can't execute action, reason: " << ex.what();
    }
}

