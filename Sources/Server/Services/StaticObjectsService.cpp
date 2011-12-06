#include <boost/foreach.hpp>

#include "Common/Logger/Logger.hpp"
#include "Server/Services/StaticObjectsService.hpp"

using namespace Server::Services;

StaticObjectsService::StaticObjectsService(Game::StaticObjectContainer & staticObjectContainer) :
    m_staticObjectContainer(staticObjectContainer)
{
}

void StaticObjectsService::handle(const Common::Messages::StaticObjectStatusReq &, Server::Network::IConnection & connection)
{
    Common::Messages::StaticObjectStatusResp resp;

    Game::StaticObjectContainer::StaticObjectVector & objects = m_staticObjectContainer.getStaticObjects();
    BOOST_FOREACH(Common::Game::StaticObject & object, objects)
    {
        resp.objects.push_back(boost::make_tuple<int>( object.getId() )); 
    }

    connection.send(resp);
}

void StaticObjectsService::handle(const Common::Messages::StaticObjectInfoReq & message, Server::Network::IConnection & connection)
{
    Common::Messages::StaticObjectInfoResp resp;

    Common::Game::StaticObject & object = m_staticObjectContainer.getStaticObject(message.staticObjectId);
    Common::Game::StaticObject::Position position = object.getPosition();
    resp.x = position.getX();
    resp.y = position.getY();
    resp.z = position.getZ();
    resp.staticObjectId = message.staticObjectId;
    connection.send(resp);
}
