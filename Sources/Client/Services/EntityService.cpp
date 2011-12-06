#include "Common/Logger/Logger.hpp"
#include "Client/Services/EntityService.hpp"

using namespace Client::Services;

EntityService::EntityService(Network::Connection & connection, 
                             Common::Game::IRustedTime & time,
                             Game::PlayerInfo & playerInfo) :
    m_playerInfo(playerInfo),
    m_entityContainer(time, m_playerInfo),
    m_connection(connection),
    m_time(time)
{
}

void EntityService::fetchMyEntitiesInfo(MyEntitiesFetchedCallback callback)
{
    m_myEntitiesFetchedCallback = callback;
    Common::Messages::PlayerEntitiesStatusReq entitiesStatusReq;
    m_connection.send(entitiesStatusReq);
}

void EntityService::setCurrentEntity(Common::Game::Entity & entity)
{
    m_currentEntity = &entity;
}

Common::Game::Entity & EntityService::getCurrentEntity()
{
    assert(m_currentEntity);
    return **m_currentEntity;
}

void EntityService::setCourse(Common::Game::Entity::Position course)
{
    assert(m_currentEntity);

    (*m_currentEntity)->setCourse(course);
    Common::Messages::EntityChangeCourseReq req;
    req.entityId = (*m_currentEntity)->getId();
    req.courseX = course.getX();
    req.courseY = course.getY();
    req.courseZ = course.getZ();
    m_connection.send(req);
}

Client::Game::EntityContainer & EntityService::getEntityContainer()
{
    return m_entityContainer;
}

void EntityService::handle(const Common::Messages::PlayerEntitiesStatusResp & entitiesStatusResp)
{
    LOG_INFO << "Own units info received\n";

    BOOST_FOREACH(boost::tuple<int> entity, entitiesStatusResp.entities)
    {
       LOG_INFO << "  Entity (id: " << entity.get<0>() << ")\n";

       Common::Messages::EntityGetInfoReq entityGetInfoReq;
       entityGetInfoReq.id = entity.get<0>();
       m_connection.send(entityGetInfoReq);
       m_myEntities.insert(entity.get<0>());
    }
}

void EntityService::handle(const Common::Messages::EntityGetInfoResp & entityGetInfoResp)
{
    LOG_INFO << "Got entity info (id: " << entityGetInfoResp.id << ")\n";

    m_entityContainer.create(entityGetInfoResp.id, 
                             entityGetInfoResp.player_id, 
                             Common::Game::Entity::Position(entityGetInfoResp.x, entityGetInfoResp.y, entityGetInfoResp.z));

    // are we waiting for this entity info?
    std::set<int>::iterator it = m_myEntities.find(entityGetInfoResp.id);
    if (it != m_myEntities.end())
    {
       LOG_INFO << "Entity is from player's entities list and we were expected this info\n";
       m_myEntities.erase(it); 

       if (m_myEntities.empty())
       {
          LOG_INFO << "And this was the last one\n";
          m_myEntitiesFetchedCallback();
       }
    }
}
