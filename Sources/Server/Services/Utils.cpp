#include "Utils.hpp"

using namespace Server::Services;

void Utils::sendObjectInfo(unsigned id, Common::Game::Universe & universe, Network::IConnection & connection)
{
    using namespace Common::Game::Object;

    ObjectBase & object = universe.getById<ObjectBase>(id);
    sendObjectInfo(object, connection);
}

void Utils::sendObjectInfo(Common::Game::Object::ObjectBase & object, Network::IConnection & connection)
{
    using namespace Common::Game::Object;

    LOG_DEBUG << "Getting info about object id: " << object.getId() << ", type: " << TYPENAME(object);

    #define IS(object, type) dynamic_cast<type*>(&object)

    if (IS(object, Ship))
    {
         sendShipInfo(dynamic_cast<Ship&>(object), connection);
    }
    else if (IS(object, StaticObject))
    {
        Common::Messages::StaticObjectInfoResp staticObjectInfo;

        Common::Game::Position position = object.getPosition();
        staticObjectInfo.staticObjectId = object.getId();
        staticObjectInfo.x = position.getX();
        staticObjectInfo.y = position.getY();
        staticObjectInfo.z = position.getZ();

        connection.send(staticObjectInfo);
    }

    #undef IS
}

void Utils::sendShipInfo(Common::Game::Object::Ship & ship, Network::IConnection & connection)
{
    Common::Messages::ShipInfo shipInfo;

    Common::Game::Position position = ship.getPosition();
    shipInfo.id = ship.getId();
    shipInfo.player_id = ship.getOwnerId();
    shipInfo.x = position.getX();
    shipInfo.y = position.getY();
    shipInfo.z = position.getZ();
    shipInfo.integrity = ship.getIntegrity();

    connection.send(shipInfo);
}