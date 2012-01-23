#include "Cake/Diagnostics/Logger.hpp"
#include "Client/Services/StaticObjectService.hpp"

using namespace Client::Services;

StaticObjectService::StaticObjectService(Network::Connection & connection, Common::Game::Universe & universe) : 
    m_connection(connection),
    m_universe(universe)
{
}

void StaticObjectService::fetchStaticObjects()
{
    Common::Messages::StaticObjectStatusReq req;
    m_connection.send(req);
}

void StaticObjectService::handle(const Common::Messages::StaticObjectStatusResp & message)
{
    LOG_DEBUG << "Static objects received";

    BOOST_FOREACH(boost::tuple<int> staticObject, message.objects)
    {
        LOG_DEBUG << "  Static object, id: " << staticObject.get<0>();

        Common::Messages::StaticObjectInfoReq req;
        req.staticObjectId = staticObject.get<0>();
        m_connection.send(req);
    }
}

void StaticObjectService::handle(const Common::Messages::StaticObjectInfoResp & message)
{
    boost::shared_ptr<Common::Game::Object::ObjectBase> object(new Common::Game::Object::StaticObject);
    object->setId(message.staticObjectId);
    object->setPosition(Common::Game::Position(message.x, message.y, message.z));
    m_universe.add(object);

    LOG_DEBUG << "New static object visible: " << *object;
}
