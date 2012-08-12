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
        auto & ship = dynamic_cast<Ship&>(object);
        sendShipInfo(ship, connection);
        sendShipCourseInfo(ship, connection);
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

void Utils::sendShipCourseInfo(Common::Game::Object::Ship & ship, Network::IConnection & connection)
{
    auto course = ship.getCourse();

    Common::Messages::ShipCourseInfo shipCourseInfo;
    shipCourseInfo.objectId = ship.getId();

    shipCourseInfo.positionX = course.start.getX();
    shipCourseInfo.positionY = course.start.getY();
    shipCourseInfo.positionZ = course.start.getZ();

    shipCourseInfo.destinationX = course.destination.getX();
    shipCourseInfo.destinationY = course.destination.getY();
    shipCourseInfo.destinationZ = course.destination.getZ();

    shipCourseInfo.startTimeSeconds = course.startTime.getSeconds();

    connection.send(shipCourseInfo);
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
    shipInfo.speed = ship.getSpeed();

    connection.send(shipInfo);
}
