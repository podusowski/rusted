#include "Cake/Diagnostics/Logger.hpp"
#include "Client/Services/StaticObjectService.hpp"

using namespace Client::Services;

StaticObjectService::StaticObjectService(Network::Connection & connection) : 
    m_connection(connection)
{
}

void StaticObjectService::setStaticObjectAddedCallback(StaticObjectService::StaticObjectAddedCallback callback)
{
    m_staticObjectAddedCallback = callback;
}

void StaticObjectService::asyncFetchStaticObjects(StaticObjectService::StaticObjectAddedCallback callback)
{
    m_staticObjectAddedCallback = callback;

    Common::Messages::StaticObjectStatusReq req;
    m_connection.send(req);
}

void StaticObjectService::handle(const Common::Messages::StaticObjectStatusResp & message)
{
    LOG_INFO << "Static objects received\n";
    BOOST_FOREACH(boost::tuple<int> staticObject, message.objects)
    {
        LOG_INFO << "  Static object, id: " << staticObject.get<0>() << "\n";
        Common::Messages::StaticObjectInfoReq req;
        req.staticObjectId = staticObject.get<0>();
        m_connection.send(req);
    }
}

void StaticObjectService::handle(const Common::Messages::StaticObjectInfoResp & message)
{
    Common::Game::Object::StaticObject object;
    object.setPosition(Common::Game::Position(message.x, message.y, message.z));
    LOG_DEBUG << "New static object: " << object << "\n";
    m_staticObjectAddedCallback(object);
}
