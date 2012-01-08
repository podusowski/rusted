#include <boost/foreach.hpp>

#include "Common/Game/Object/StaticObject.hpp"
#include "Server/Services/StaticObjectsService.hpp"

using namespace Server::Services;

StaticObjectsService::StaticObjectsService(Common::Game::Universe & universe) :
    m_universe(universe)
{
}

void StaticObjectsService::handle(const Common::Messages::StaticObjectStatusReq &, Server::Network::IConnection & connection)
{
    Common::Messages::StaticObjectStatusResp resp;

    Common::Game::Universe::Objects objects = m_universe.get<Common::Game::Object::StaticObject>(); 
    BOOST_FOREACH(boost::shared_ptr<Common::Game::Object::ObjectBase> object, objects)
    {
        resp.objects.push_back(boost::make_tuple<int>( object->getId() )); 
    }

    connection.send(resp);
}

void StaticObjectsService::handle(const Common::Messages::StaticObjectInfoReq & message, Server::Network::IConnection & connection)
{
    Common::Messages::StaticObjectInfoResp resp;

    Common::Game::Object::StaticObject & object = m_universe.getById<Common::Game::Object::StaticObject>(message.staticObjectId);
    Common::Game::Position position = object.getPosition();
    resp.x = position.getX();
    resp.y = position.getY();
    resp.z = position.getZ();
    resp.staticObjectId = message.staticObjectId;
    connection.send(resp);
}
