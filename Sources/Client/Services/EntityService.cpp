#include "Cake/Diagnostics/Logger.hpp"

#include "Common/Game/Object/Ship.hpp"
#include "Client/Services/EntityService.hpp"

using namespace Client::Services;

EntityService::EntityService(Network::IConnection & connection, 
                             Game::PlayerInfo & playerInfo,
                             Common::Game::Universe & universe) :
    m_playerInfo(playerInfo),
    m_connection(connection),
    m_universe(universe)
{
}

void EntityService::setCurrentEntity(Common::Game::Object::Ship & ship)
{
    m_currentShip = &ship;
}

Common::Game::Object::Ship & EntityService::getCurrentEntity()
{
    assert(m_currentShip);
    return **m_currentShip;
}

void EntityService::setCourse(Common::Game::Position course)
{
    assert(m_currentShip);

    (*m_currentShip)->setCourse(course);
    Common::Messages::EntityChangeCourseReq req;
    req.entityId = (*m_currentShip)->getId();
    req.courseX = course.getX();
    req.courseY = course.getY();
    req.courseZ = course.getZ();
    m_connection.send(req);
}

