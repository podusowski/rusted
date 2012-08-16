#include "Common/Game/Object/Ship.hpp"
#include "Common/Game/Object/StaticObject.hpp"

#include "Server/Services/EntityService.hpp"

using namespace Server::Services;

EntityService::EntityService(Common::Game::Universe & universe, Server::Game::PlayerContainer & playerContainer) :
    m_universe(universe),
    m_playerContainer(playerContainer),
    m_actionBuilder(universe, playerContainer)
{
}

void EntityService::handle(const Common::Messages::EntityChangeCourseReq & changeCourseReq, Network::IConnection & connection)
{
    Common::Game::Object::Ship & ship = m_universe.getById<Common::Game::Object::Ship>(changeCourseReq.entityId);

    Common::Game::Position destination(changeCourseReq.courseX,
                                       changeCourseReq.courseY,
                                       changeCourseReq.courseZ);
    ship.setCourse(destination);

    std::vector<Network::IConnection *> connections = m_playerContainer.getAllConnections(Server::Game::PLAYER_STATE_AUTHORIZED);
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

        visibleObjects.objects.push_back(boost::make_tuple(object->getId()));
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
    Common::Messages::AvailableActions availableActions;
    availableActions.actions.push_back(boost::make_tuple<int, std::string>(1, "attack"));
    availableActions.actions.push_back(boost::make_tuple<int, std::string>(2, "buildShip"));
    connection.send(availableActions);
}

void EntityService::handle(const Common::Messages::ExecuteAction & executeAction, Network::IConnection & connection)
{
    auto & player = m_playerContainer.getBy(connection);

    LOG_DEBUG << "Player " << player.getId() << " is executing action " << executeAction.id;

    try
    {
        auto action = m_actionBuilder.build(connection, player, executeAction.id);
        action->execute();
    }
    catch (std::exception ex)
    {
        LOG_DEBUG << "Can't build or execute action, reason: " << ex.what();
    }
}

