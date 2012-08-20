#include "Cake/Diagnostics/Logger.hpp"

#include "Common/Game/Object/Ship.hpp"
#include "Client/Services/PlayerActionService.hpp"

using namespace Client::Services;

PlayerActionService::PlayerActionService(Network::IConnection & connection, 
                             Game::PlayerInfo & playerInfo,
                             Common::Game::Universe & universe) :
    m_playerInfo(playerInfo),
    m_connection(connection),
    m_universe(universe)
{
}

void PlayerActionService::focusObject(Common::Game::Object::ObjectBase & object)
{
    LOG_DEBUG << "Object focused: " << object.getId();

    Common::Messages::FocusObject focusObject;
    focusObject.id = object.getId();
    m_connection.send(focusObject);

    m_focusedObject = &object;
}

Common::Game::Object::ObjectBase & PlayerActionService::getFocusedObject()
{
    assert(m_focusedObject);

    return **m_focusedObject;
}

void PlayerActionService::setFocusedObjectCourse(Common::Game::Position course)
{
    assert(m_focusedObject);

    dynamic_cast<Common::Game::Object::Ship&>(**m_focusedObject).setCourse(course);

    Common::Messages::EntityChangeCourseReq req;

    req.entityId = (*m_focusedObject)->getId();
    req.courseX = course.getX();
    req.courseY = course.getY();
    req.courseZ = course.getZ();

    m_connection.send(req);
}

void PlayerActionService::selectObject(Common::Game::Object::ObjectBase & object)
{
    LOG_DEBUG << "Object selected: " << object.getId();

    Common::Messages::SelectObject selectObject;
    selectObject.id = object.getId();
    m_connection.send(selectObject);

    m_selectedObject = &object;
}

void PlayerActionService::fetchAvailableActions(AvailableActionsFetchedCallback callback)
{
    LOG_DEBUG << "Fetching available actions";

    m_availableActionsFetchedCallback = callback;

    Common::Messages::FetchAvailableActions fetchAvailableActions;
    m_connection.send(fetchAvailableActions);
}

void PlayerActionService::executeAction(unsigned actionId, GlobalCooldownExpiredCallback callback)
{
    assert(m_focusedObject);

    LOG_DEBUG << "Performing action: " << actionId;

    m_globalCooldownExpiredCallback = callback;

    Common::Messages::ExecuteAction executeAction;
    executeAction.id = actionId;

    m_connection.send(executeAction);
}

void PlayerActionService::handle(const Common::Messages::AvailableActions & availableActions)
{
    LOG_DEBUG << "Got available actions";

    if (m_availableActionsFetchedCallback)
    {
        m_availableActionsFetchedCallback(availableActions.actions);
    }
}

void PlayerActionService::handle(const Common::Messages::GlobalCooldownExpired &)
{
    LOG_DEBUG << "Global cooldown expired";

    if (m_globalCooldownExpiredCallback)
    {
        m_globalCooldownExpiredCallback();
    }
}

