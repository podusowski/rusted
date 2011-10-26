#include "Common/Logger/Logger.hpp"
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

void StaticObjectService::fetchStaticObjects()
{
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
    Common::Game::StaticObject object(message.staticObjectId, Common::Game::StaticObject::Position(message.x, message.y, message.z));
    m_staticObjectAddedCallback(object);
}
