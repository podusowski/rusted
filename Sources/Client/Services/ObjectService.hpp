#pragma once

#include "Common/Game/Universe.hpp"
#include "Services/AbstractService.hpp"
#include "Network/IConnection.hpp"

namespace Client
{
namespace Services
{

class ObjectService : public AbstractService<ObjectService> 
{
public:
    ObjectService(Network::IConnection &, Common::Game::Universe &);

    void fetchVisibleObjects();

    void handle(const Common::Messages::VisibleObjects &);
    void handle(const Common::Messages::ShipInfo &);
    void handle(const Common::Messages::AbstractMessage &) {}

private:
    Network::IConnection & m_connection;
    Common::Game::Universe & m_universe;
};

}
}
