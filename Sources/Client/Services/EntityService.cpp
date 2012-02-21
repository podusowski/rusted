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

void EntityService::fetchMyEntitiesInfo(MyEntitiesFetchedCallback callback)
{
    m_myEntitiesFetchedCallback = callback;
    Common::Messages::PlayerEntitiesStatusReq entitiesStatusReq;
    m_connection.send(entitiesStatusReq);
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

void EntityService::handle(const Common::Messages::PlayerEntitiesStatusResp & entitiesStatusResp)
{
    LOG_DEBUG << "Own units info received";

    BOOST_FOREACH(boost::tuple<int> entity, entitiesStatusResp.entities)
    {
       LOG_DEBUG << "  Entity (id: " << entity.get<0>() << ")";

       Common::Messages::EntityGetInfoReq entityGetInfoReq;
       entityGetInfoReq.id = entity.get<0>();
       m_connection.send(entityGetInfoReq);
       m_myEntities.insert(entity.get<0>());
    }
}

void EntityService::handle(const Common::Messages::ShipInfo & entityGetInfoResp)
{
    // are we waiting for this entity info?
    std::set<int>::iterator it = m_myEntities.find(entityGetInfoResp.id);
    if (it != m_myEntities.end())
    {
       LOG_DEBUG << "Ship is from player's ship list and we were expected this info";
       m_myEntities.erase(it); 

       if (m_myEntities.empty())
       {
          LOG_DEBUG << "And this was the last one";
          m_myEntitiesFetchedCallback();
       }
    }
}

void EntityService::handle(const Common::Messages::EntityChangeCourseReq & entityChangeCourse)
{
    LOG_DEBUG << "Ship " << entityChangeCourse.entityId << " changed course";

    Common::Game::Object::Ship & ship = m_universe.getById<Common::Game::Object::Ship>(entityChangeCourse.entityId);
    ship.setCourse(Common::Game::Position(
        entityChangeCourse.courseX,
        entityChangeCourse.courseY,
        entityChangeCourse.courseZ));
}
