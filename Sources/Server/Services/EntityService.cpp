#include "Common/Game/Object/Ship.hpp"

#include "Server/Services/EntityService.hpp"

using namespace Server::Services;

EntityService::EntityService(Common::Game::Universe & universe) :
    m_universe(universe)
{
}

void EntityService::handle(const Common::Messages::EntityGetInfoReq & getInfoReq, Network::IConnection & connection)
{
    Common::Game::Object::ObjectBase & ship = m_universe.getById<Common::Game::Object::Ship>(getInfoReq.id);
    Common::Point3<int> position = ship.getPosition();
    Common::Messages::EntityGetInfoResp getInfoResp;
    getInfoResp.id = getInfoReq.id;
    getInfoResp.player_id = dynamic_cast<Common::Game::Object::Ship&>(ship).getOwnerId();
    getInfoResp.x = position.getX();
    getInfoResp.y = position.getY();
    getInfoResp.z = position.getZ();
    connection.send(getInfoResp);
}

void EntityService::handle(const Common::Messages::EntityChangeCourseReq & changeCourseReq, Network::IConnection &)
{
    Common::Game::Object::Ship & ship = m_universe.getById<Common::Game::Object::Ship>(changeCourseReq.entityId);
    Common::Point3<int> destination(changeCourseReq.courseX,
                                    changeCourseReq.courseY,
                                    changeCourseReq.courseZ);
    ship.setCourse(destination);
}
