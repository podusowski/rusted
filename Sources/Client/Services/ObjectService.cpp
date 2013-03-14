#include "Common/Game/Object/Ship.hpp"
#include "Common/Game/Object/Asteroid.hpp"
#include "ObjectService.hpp"

using namespace Client::Services;

ObjectService::ObjectService(Client::Network::IConnection & connection, Common::Game::Universe & universe) :
    m_connection(connection),
    m_universe(universe)
{
}

void ObjectService::fetchVisibleObjects()
{
    LOG_DEBUG << "Fetching visible objects";

    Common::Messages::GetVisibleObjects getVisibleObjects;
    m_connection.send(getVisibleObjects);
}

void ObjectService::fetchPlayerShips(PlayerShipsFetchedCallback callback)
{
    m_playerShipsFetchedCallback = callback;
    Common::Messages::FetchPlayerShips fetchPlayerShips;
    m_connection.send(fetchPlayerShips);
}

void ObjectService::handle(const Common::Messages::VisibleObjects & visibleObjects)
{
    LOG_DEBUG << "Got visible objects";

    for (auto object: visibleObjects.objects)
    {
        LOG_DEBUG << "Fetch info about object id: " << object.id;

        //TODO we might have it already

        Common::Messages::GetObjectInfo getObjectInfo;
        getObjectInfo.id = object.id;
        m_connection.send(getObjectInfo);
    }
}

void ObjectService::handle(const Common::Messages::PlayerShips & playerShips)
{
    LOG_DEBUG << "Got player ships";

    for (auto ship: playerShips.ships)
    {
        int shipId = ship.id;

        LOG_DEBUG << "  id: " << shipId;

        m_playerShips.insert(shipId);

        Common::Messages::GetObjectInfo getObjectInfo;
        getObjectInfo.id = shipId;
        m_connection.send(getObjectInfo);
    }
}

// FIXME: when ship is currently on some course and we update it with ShipInfo,
//        it will stop at currect position
void ObjectService::handle(const Common::Messages::ShipInfo & shipInfo)
{
    if (m_universe.has(shipInfo.id))
    {
        LOG_DEBUG << "Updating ship (id: " << shipInfo.id << ")";

        auto & ship = m_universe.getById<Common::Game::Object::Ship>(shipInfo.id);
        ship.setOwnerId(shipInfo.player_id);
        ship.setPosition(Common::Game::Position(shipInfo.x, shipInfo.y, shipInfo.z));
        ship.setIntegrity(shipInfo.integrity);
        ship.setMesh(shipInfo.mesh);

        tryCallPlayerShipsFetchedCallback(ship.getId());
    }
    else
    {
        LOG_DEBUG << "New ship visible (id: " << shipInfo.id << ")";

        boost::shared_ptr<Common::Game::Object::ObjectBase> object(new Common::Game::Object::Ship);
        Common::Game::Object::Ship & ship = dynamic_cast<Common::Game::Object::Ship&>(*object);
        ship.setId(shipInfo.id);
        ship.setOwnerId(shipInfo.player_id);
        ship.setPosition(Common::Game::Position(shipInfo.x, shipInfo.y, shipInfo.z));
        ship.setIntegrity(shipInfo.integrity);
        ship.setSpeed(shipInfo.speed);
        ship.setMesh(shipInfo.mesh);
        m_universe.add(object);

        tryCallPlayerShipsFetchedCallback(object->getId());
    }
}

// TODO: if object is present, we need to update it
void ObjectService::handle(const Common::Messages::AsteroidInfo & message)
{
    try
    {
        boost::shared_ptr<Common::Game::Object::ObjectBase> object(new Common::Game::Object::Asteroid);
    
        object->setId(message.objectId);
        object->setPosition(Common::Game::Position(message.x, message.y, message.z));

        m_universe.add(object);

        LOG_DEBUG << "New asteroid visible: " << TYPENAME(*object);
    }
    catch (std::exception & ex)
    {
        LOG_WARN << "Can't insert object, reason: " << ex.what();
    }
}

void ObjectService::handle(const Common::Messages::ShipCourseInfo & shipCourseInfo)
{
    LOG_DEBUG << "Ship (id: " << shipCourseInfo.objectId << ") changed course";

    Common::Game::Object::Ship & ship = m_universe.getById<Common::Game::Object::Ship>(shipCourseInfo.objectId);

    Common::Game::Object::IFlightTrajectory::Description description;

    for (const auto & p : shipCourseInfo.controlPoints)
    {
        Common::Game::Position position(p.x, p.y, p.z);
        description.controlPoints.push_back(position);
    }

    description.startTime = Common::Game::TimeValue(
        shipCourseInfo.startTimeSeconds,
        shipCourseInfo.startTimeMiliseconds);

    ship.applyTrajectoryDescription(description);
}

void ObjectService::handle(const Common::Messages::ObjectCargoInfo & objectCargoInfo)
{
    LOG_DEBUG << "Got ObjectCargoInfo for object:" << objectCargoInfo.id;

    auto & object = m_universe.getById<Common::Game::Object::ObjectBase>(objectCargoInfo.id);
    object.visitCargoHold([&objectCargoInfo](Common::Game::Object::CargoHold & cargoHold) -> void
    {
        cargoHold.setCapacity(objectCargoInfo.capacity);
        cargoHold.setCarbon(objectCargoInfo.carbon);
        cargoHold.setHelium(objectCargoInfo.helium);
    });
}

void ObjectService::tryCallPlayerShipsFetchedCallback(int shipId)
{
    auto it = m_playerShips.find(shipId);
    if (it != m_playerShips.end())
    {
        LOG_DEBUG << "Received player's ship info (id: " << shipId << ") which was expected";

        m_playerShips.erase(it);

        if (m_playerShips.empty())
        {
            LOG_DEBUG << "All player ships are known, calling PlayerShipsFetchedCallback";

            m_playerShipsFetchedCallback();
        }
    }
}
