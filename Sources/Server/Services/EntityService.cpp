#include "Common/Game/Object/Ship.hpp"
#include "Common/Game/Object/StaticObject.hpp"

#include "Game/Actions/ActionType.hpp"
#include "Server/Services/EntityService.hpp"

using namespace Server::Services;

EntityService::EntityService(Common::Game::Universe & universe, Server::Game::PlayerContainer & playerContainer) :
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
    availableActions.actions.push_back(boost::make_tuple<int, int, std::string>(Game::Actions::ActionType_Attack, 1, "attack"));
    availableActions.actions.push_back(boost::make_tuple<int, int, std::string>(Game::Actions::ActionType_BuildShip, 1, "buildShip"));
    connection.send(availableActions);
}

void EntityService::handle(const Common::Messages::ExecuteAction & executeAction, Network::IConnection & connection)
{
    auto & player = m_playerContainer.getBy(connection);

    LOG_DEBUG << "Player " << player.getId() << " is executing action " << executeAction.id;

    try
    {
        m_actionPerformer.perform(connection, player, executeAction.id);
    }
    catch (std::exception ex)
    {
        LOG_DEBUG << "Can't execute action, reason: " << ex.what();
    }
}

