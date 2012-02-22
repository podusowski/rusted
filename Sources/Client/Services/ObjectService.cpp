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

// TODO: if object is present, we need to update it
void ObjectService::handle(const Common::Messages::ShipInfo & shipInfo)
{
    try
    {
        boost::shared_ptr<Common::Game::Object::ObjectBase> object(new Common::Game::Object::Ship);
        Common::Game::Object::Ship & ship = dynamic_cast<Common::Game::Object::Ship&>(*object);
        ship.setId(shipInfo.id);
        ship.setOwnerId(shipInfo.player_id);
        ship.setPosition(Common::Game::Position(shipInfo.x, shipInfo.y, shipInfo.z));
        m_universe.add(object);

        LOG_DEBUG << "New ship visible (id: " << shipInfo.id << ")";
    }
    catch (std::exception & ex)
    {
        LOG_WARN << "Can't insert object, reason: " << ex.what();
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
