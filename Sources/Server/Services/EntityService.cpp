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
    Common::Messages::ShipInfo shipInfo;
    shipInfo.id = getInfoReq.id;
    shipInfo.player_id = dynamic_cast<Common::Game::Object::Ship&>(ship).getOwnerId();
    shipInfo.x = position.getX();
    shipInfo.y = position.getY();
    shipInfo.z = position.getZ();
    connection.send(shipInfo);
}

void EntityService::handle(const Common::Messages::EntityChangeCourseReq & changeCourseReq, Network::IConnection &)
{
    Common::Game::Object::Ship & ship = m_universe.getById<Common::Game::Object::Ship>(changeCourseReq.entityId);
    Common::Point3<int> destination(changeCourseReq.courseX,
                                    changeCourseReq.courseY,
                                    changeCourseReq.courseZ);
    ship.setCourse(destination);
}
