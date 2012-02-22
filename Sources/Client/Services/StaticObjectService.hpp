#pragma once

#include "Common/Game/Universe.hpp"
#include "Common/Game/Object/StaticObject.hpp"
#include "Client/Services/AbstractService.hpp"
#include "Client/Network/IConnection.hpp"

namespace Client
{
namespace Services
{

class StaticObjectService : public AbstractService<StaticObjectService>
{
public:
    StaticObjectService(Network::IConnection &, Common::Game::Universe & universe);

    void fetchStaticObjects();

    void handle(const Common::Messages::StaticObjectStatusResp &);
    void handle(const Common::Messages::AbstractMessage &) {}

private:
    Network::IConnection & m_connection;
    Common::Game::Universe & m_universe;
};

}
}
