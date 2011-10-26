#include "Server/Services/EntityService.hpp"

using namespace Server::Services;

EntityService::EntityService(Common::Game::EntityContainer & entities) :
    m_entities(entities)
{
}

void EntityService::handle(const Common::Messages::EntityGetInfoReq & getInfoReq, Network::IConnection & connection)
{
    Common::Game::Entity & entity = m_entities.getById(getInfoReq.id);
    Common::Point3<int> position = entity.getPosition();
    Common::Messages::EntityGetInfoResp getInfoResp;
    getInfoResp.id = getInfoReq.id;
    getInfoResp.player_id = entity.getPlayerId();
    getInfoResp.x = position.getX();
    getInfoResp.y = position.getY();
    getInfoResp.z = position.getZ();
    connection.send(getInfoResp);
}

void EntityService::handle(const Common::Messages::EntityChangeCourseReq & changeCourseReq, Network::IConnection &)
{
    Common::Game::Entity & entity = m_entities.getById(changeCourseReq.entityId);
    Common::Point3<int> destination(changeCourseReq.courseX,
                                    changeCourseReq.courseY,
                                    changeCourseReq.courseZ);
    entity.setCourse(destination);
}
