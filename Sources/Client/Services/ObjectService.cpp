#include "Common/Game/Object/Ship.hpp"
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

void ObjectService::handle(const Common::Messages::ShipInfo & shipInfo)
{
    LOG_DEBUG << "Got ship info (id: " << shipInfo.id << ")";

    boost::shared_ptr<Common::Game::Object::ObjectBase> object(new Common::Game::Object::Ship);
    Common::Game::Object::Ship & ship = dynamic_cast<Common::Game::Object::Ship&>(*object);
    ship.setId(shipInfo.id);
    ship.setOwnerId(shipInfo.player_id);
    ship.setPosition(Common::Game::Position(shipInfo.x, shipInfo.y, shipInfo.z));

    m_universe.add(object);
}
