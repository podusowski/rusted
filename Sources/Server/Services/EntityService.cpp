#include "Common/Game/Object/Ship.hpp"

#include "Game/Actions/ActionType.hpp"
#include "Server/Services/EntityService.hpp"
#include "Server/Game/Actions/ActionParameters.hpp"

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
    // TODO: thread safety
    Common::Game::Object::Ship & ship = m_universe.getById<Common::Game::Object::Ship>(changeCourseReq.shipId);

    const Common::Game::Position destination(
        changeCourseReq.x,
        changeCourseReq.y,
        changeCourseReq.z);

    ship.setCourse(destination);

    // TODO: thread safety
    std::vector<Network::IConnection *> connections = m_playerContainer.getAllConnections(Common::Game::PLAYER_STATE_AUTHORIZED);
    for (std::vector<Network::IConnection *>::iterator it = connections.begin(); it != connections.end(); it++)
    {
        //if (*it != &connection)
        {
            m_utils.sendShipCourseInfo(ship, **it);
        }
    }
}

void EntityService::handle(const Common::Messages::GetVisibleObjects &, Network::IConnection & connection)
{
    m_utils.sendVisibleObjects(m_universe, connection);
}

void EntityService::handle(const Common::Messages::GetObjectInfo & getObjectInfo, Network::IConnection & connection)
{
    m_utils.sendObjectInfo(getObjectInfo.id, m_universe, connection);
}

void EntityService::handle(const Common::Messages::SelectObject & selectObject, Network::IConnection & connection)
{
    m_playerContainer.invokeOnPlayer(connection, [&] (Common::Game::IPlayer & player)
    {
        auto & object = m_universe.getById<Common::Game::Object::ObjectBase>(selectObject.id);

        LOG_DEBUG << "Player " << player.getId() << " is selecting " << object;

        player.selectObject(object);
    });
}

void EntityService::handle(const Common::Messages::FocusObject & focusObject, Network::IConnection & connection)
{
    m_playerContainer.invokeOnPlayer(connection, [&] (Common::Game::IPlayer & player)
    {
        auto & object = m_universe.getById<Common::Game::Object::ObjectBase>(focusObject.id);

        LOG_DEBUG << "Player " << player.getId() << " is focusing " << object;

        player.focusObject(object);
    });
}

void EntityService::handle(const Common::Messages::FetchAvailableActions &, Network::IConnection & connection)
{
    m_playerContainer.invokeOnPlayer(connection, [&] (Common::Game::IPlayer & player)
    {
        // TODO: thread safety
        auto & object = dynamic_cast<Common::Game::Object::Ship&>(player.getFocusedObject());
        auto & shipClass = m_shipClassContainer->getById(object.getClass());
        auto actions = shipClass.getAvailableActions();

        Common::Game::Object::ObjectBase::StrictId focusedObjectId = player.getFocusedObjectId().get();
        Common::Game::Object::ObjectBase::Id selectedObjectId = player.getSelectedObjectId();

        Common::Messages::AvailableActions availableActions;
        LOG_DEBUG << "Filling available actions for ship:" << object.getId() << " (class:" << shipClass.getId() << ")";
        for (auto a: actions)
        {
            Common::Game::IPlayer::Id playerId(player.getId());
            Game::Actions::ActionParameters actionParameters(playerId, a.type, a.parameter, focusedObjectId, selectedObjectId);

            const auto action = m_actionFactory.create(connection, player, actionParameters);

            LOG_DEBUG << "  name:" << action->getName()
                      << ", description: " << action->getDescription()
                      << ", type:" << a.type
                      << ", parameter:" << a.parameter;

            Common::Messages::AvailableAction availableAction;
            availableAction.id = a.type;
            availableAction.name = action->getName();
            availableAction.parameter = a.parameter;
            availableAction.description = action->getDescription();

            availableActions.actions.push_back(availableAction);
        }
        connection.send(availableActions);
    });
}

void EntityService::handle(const Common::Messages::ExecuteAction & executeAction, Network::IConnection & connection)
{
    m_playerContainer.invokeOnPlayer(connection, [&] (Common::Game::IPlayer & player)
    {
        Common::Game::Object::ObjectBase::StrictId focusedObjectId = player.getFocusedObjectId().get();
        Common::Game::Object::ObjectBase::Id selectedObjectId = player.getSelectedObjectId();

        try
        {
            Common::Game::IPlayer::Id playerId(player.getId());
            Game::Actions::ActionParameters actionParameters(playerId, executeAction.id, executeAction.parameter, focusedObjectId, selectedObjectId);

            m_actionPerformer.perform(connection, player, actionParameters, executeAction.loop);
        }
        catch (const std::exception & ex)
        {
            LOG_DEBUG << "Can't execute action, reason: " << ex.what();
        }
    });
}

void EntityService::handle(const Common::Messages::GetPlayerName & getPlayerName, Network::IConnection & connection)
{
    Common::Messages::PlayerName playerName; 
    auto playerSummary = m_playerContainer.getPlayerSummary(getPlayerName.id);
    playerName.id = getPlayerName.id;
    playerName.name = playerSummary.name;
    connection.send(playerName);
}

