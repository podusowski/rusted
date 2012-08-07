#include "Common/Game/Object/Ship.hpp"
#include "Common/Game/Object/StaticObject.hpp"
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
    Common::Messages::PlayerEntitiesStatusReq playerEntitiesStatusReq;
    m_connection.send(playerEntitiesStatusReq);
}

void ObjectService::handle(const Common::Messages::VisibleObjects & visibleObjects)
{
    LOG_DEBUG << "Got visible objects";

    for (auto object: visibleObjects.objects)
    {
        LOG_DEBUG << "Fetch info about object id: " << object.get<0>();

        //TODO we might have it already

        Common::Messages::GetObjectInfo getObjectInfo;
        getObjectInfo.id = object.get<0>();
        m_connection.send(getObjectInfo);
    }
}

void ObjectService::handle(const Common::Messages::PlayerEntitiesStatusResp & playerEntitiesStatusResp)
{
    LOG_DEBUG << "Got player ships";

    for (auto ship: playerEntitiesStatusResp.entities)
    {
        int shipId = ship.get<0>();

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
        auto & ship = m_universe.getById<Common::Game::Object::Ship>(shipInfo.id);
        ship.setOwnerId(shipInfo.player_id);
        ship.setPosition(Common::Game::Position(shipInfo.x, shipInfo.y, shipInfo.z));
        ship.setIntegrity(shipInfo.integrity);
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
        m_universe.add(object);

        tryCallPlayerShipsFetchedCallback(object->getId());
    }
}

// TODO: if object is present, we need to update it
void ObjectService::handle(const Common::Messages::StaticObjectInfoResp & message)
{
    try
    {
        boost::shared_ptr<Common::Game::Object::ObjectBase> object(new Common::Game::Object::StaticObject);
        object->setId(message.staticObjectId);
        object->setPosition(Common::Game::Position(message.x, message.y, message.z));
        m_universe.add(object);

        LOG_DEBUG << "New static object visible: " << TYPENAME(*object);
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

    ship.setCourse(Common::Game::Position(
        shipCourseInfo.destinationX,
        shipCourseInfo.destinationY,
        shipCourseInfo.destinationZ));
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
