#pragma once

#include "Common/Game/Universe.hpp"
#include "Common/Game/Object/StaticObject.hpp"
#include "Client/Services/AbstractService.hpp"
#include "Client/Network/Connection.hpp"

namespace Client
{
namespace Services
{

class StaticObjectService : public AbstractService<StaticObjectService>
{
public:
    StaticObjectService(Network::Connection &, Common::Game::Universe & universe);

    void fetchStaticObjects();

    void handle(const Common::Messages::StaticObjectStatusResp &);
    void handle(const Common::Messages::StaticObjectInfoResp &);
    void handle(const Common::Messages::AbstractMessage &) {}

private:
    Network::Connection & m_connection;
    Common::Game::Universe & m_universe;
};

}
}
