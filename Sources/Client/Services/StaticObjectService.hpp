#pragma once

#include <boost/function.hpp>

#include "Common/Game/StaticObject.hpp"
#include "Client/Services/AbstractService.hpp"
#include "Client/Network/Connection.hpp"

namespace Client
{
namespace Services
{

class StaticObjectService : public AbstractService<StaticObjectService>
{
public:
    typedef boost::function<void(Common::Game::StaticObject &)> StaticObjectAddedCallback;

    StaticObjectService(Network::Connection &);

    void setStaticObjectAddedCallback(StaticObjectAddedCallback);
    void asyncFetchStaticObjects(StaticObjectService::StaticObjectAddedCallback callback);

    void handle(const Common::Messages::StaticObjectStatusResp &);
    void handle(const Common::Messages::StaticObjectInfoResp &);
    void handle(const Common::Messages::AbstractMessage &) {}

private:
    StaticObjectAddedCallback m_staticObjectAddedCallback;
    Network::Connection & m_connection;
};

}
}
