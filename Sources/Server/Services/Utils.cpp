#include "Utils.hpp"

#include "Common/Game/Object/Asteroid.hpp"

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

    if (object.is<Ship>())
    {
        auto & ship = dynamic_cast<Ship&>(object);
        sendShipInfo(ship, connection);
        sendShipCourseInfo(ship, connection);
        sendObjectCargoInfo(ship, connection);
    }
    else if (object.is<Asteroid>())
    {
        Common::Messages::AsteroidInfo asteroidInfo;

        auto & asteroid = dynamic_cast<Common::Game::Object::Asteroid&>(object);

        Common::Game::Position position = object.getPosition();
        asteroidInfo.objectId = object.getId();
        asteroidInfo.x = position.getX();
        asteroidInfo.y = position.getY();
        asteroidInfo.z = position.getZ();

        connection.send(asteroidInfo);

        sendObjectCargoInfo(asteroid, connection);
    }
}

void Utils::sendShipCourseInfo(Common::Game::Object::Ship & ship, Network::IConnection & connection)
{
    auto description = ship.getTrajectoryDescription();

    Common::Messages::ShipCourseInfo shipCourseInfo;
    shipCourseInfo.objectId = ship.getId();

    shipCourseInfo.positionX = description.start.getX();
    shipCourseInfo.positionY = description.start.getY();
    shipCourseInfo.positionZ = description.start.getZ();

    shipCourseInfo.destinationX = description.destination.getX();
    shipCourseInfo.destinationY = description.destination.getY();
    shipCourseInfo.destinationZ = description.destination.getZ();

    shipCourseInfo.startTimeSeconds = description.startTime.getSeconds();
    shipCourseInfo.startTimeMiliseconds = description.startTime.getMiliseconds();

    connection.send(shipCourseInfo);
}

void Utils::sendShipInfo(Common::Game::Object::Ship & ship, Network::IConnection & connection)
{
    Common::Messages::ShipInfo shipInfo;

    Common::Game::Position position = ship.getPosition();
    shipInfo.id = ship.getId();
    shipInfo.player_id = ship.getOwnerId();
    shipInfo.mesh = ship.getMesh();
    shipInfo.x = position.getX();
    shipInfo.y = position.getY();
    shipInfo.z = position.getZ();
    shipInfo.integrity = ship.getIntegrity();
    shipInfo.speed = ship.getSpeed();

    connection.send(shipInfo);
}

void Utils::sendObjectCargoInfo(Common::Game::Object::ObjectBase & object, Network::IConnection & connection)
{
    Common::Messages::ObjectCargoInfo objectCargoInfo;
    objectCargoInfo.id = object.getId();

    object.visitCargoHold([&objectCargoInfo](Common::Game::Object::CargoHold & cargoHold) -> void
    {
        objectCargoInfo.capacity = cargoHold.getCapacity();
        objectCargoInfo.carbon = cargoHold.getCarbon();
        objectCargoInfo.helium = cargoHold.getHelium();
    });

    connection.send(objectCargoInfo);
}

