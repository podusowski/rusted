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

    m_selectedObject = &object;
}

void PlayerActionService::fetchAvailableActions()
{
    LOG_DEBUG << "Fetching available actions";

    Common::Messages::FetchAvailableActions fetchAvailableActions;
    m_connection.send(fetchAvailableActions);
}

void PlayerActionService::handle(const Common::Messages::AvailableActions & availableActions)
{
    LOG_DEBUG << "Got available actions";
}
