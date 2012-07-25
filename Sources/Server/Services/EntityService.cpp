#include "Common/Game/Object/Ship.hpp"
#include "Common/Game/Object/StaticObject.hpp"

#include "Server/Services/EntityService.hpp"

using namespace Server::Services;

EntityService::EntityService(Common::Game::Universe & universe, Server::Game::PlayerContainer & playerContainer) :
    m_universe(universe),
    m_playerContainer(playerContainer)
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

void EntityService::handle(const Common::Messages::EntityChangeCourseReq & changeCourseReq, Network::IConnection & connection)
{
    Common::Game::Object::Ship & ship = m_universe.getById<Common::Game::Object::Ship>(changeCourseReq.entityId);
    Common::Point3<int> destination(changeCourseReq.courseX,
                                    changeCourseReq.courseY,
                                    changeCourseReq.courseZ);
    ship.setCourse(destination);

    std::vector<Network::IConnection *> connections = m_playerContainer.getAllConnections(Server::Game::PLAYER_STATE_AUTHORIZED);
    for (std::vector<Network::IConnection *>::iterator it = connections.begin(); it != connections.end(); it++)
    {
        if (*it != &connection)
        {
            (*it)->send(changeCourseReq);
        }
    }
}

void EntityService::handle(const Common::Messages::GetVisibleObjects &, Network::IConnection & connection)
{
    Common::Messages::VisibleObjects visibleObjects;

    LOG_DEBUG << "Filling visible objects";

    auto objects = m_universe.getAll(); 
    for (auto object: objects)
    {
        LOG_DEBUG << "  id: " << object->getId() << ", type: " << TYPENAME(*object);

        visibleObjects.objects.push_back(boost::make_tuple(object->getId()));
    }

    connection.send(visibleObjects);
}

void EntityService::handle(const Common::Messages::GetObjectInfo & getObjectInfo, Network::IConnection & connection)
{
    using namespace Common::Game::Object;

    ObjectBase & object = m_universe.getById<ObjectBase>(getObjectInfo.id);

    LOG_DEBUG << "Getting info about object id: " << getObjectInfo.id << ", type: " << TYPENAME(object);

    if (typeid(object) == typeid(Ship))
    {
         sendShipInfo(dynamic_cast<Ship&>(object), connection);
    }
    else if (typeid(object) == typeid(StaticObject))
    {
        Common::Messages::StaticObjectInfoResp staticObjectInfo;

        Common::Game::Position position = object.getPosition();
        staticObjectInfo.staticObjectId = getObjectInfo.id;
        staticObjectInfo.x = position.getX();
        staticObjectInfo.y = position.getY();
        staticObjectInfo.z = position.getZ();

        connection.send(staticObjectInfo);
    }
}

void EntityService::handle(const Common::Messages::AttackObject & attackObject, Network::IConnection & connection)
{
    auto & attacker = m_universe.getById<Common::Game::Object::Ship>(attackObject.attackerId);
    auto & attacked = m_universe.getById<Common::Game::Object::Ship>(attackObject.attackedId);

    LOG_DEBUG << attacker << " is attacking " << attacked;

    //TODO: select proper attack
    attacker.attack(0, attacked);

    auto connections = m_playerContainer.getAllConnections(Server::Game::PLAYER_STATE_AUTHORIZED);
    for (auto connection: connections)
    {
        connection->send(attackObject);
        sendShipInfo(attacked, *connection);
    }
}

void EntityService::handle(const Common::Messages::FetchAvailableActions &, Network::IConnection & connection)
{
    Common::Messages::AvailableActions availableActions;
    availableActions.actions.push_back(boost::make_tuple<int, std::string>(1, "attack"));
    connection.send(availableActions);
}

void EntityService::sendShipInfo(Common::Game::Object::Ship & ship, Network::IConnection & connection)
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

