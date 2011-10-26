#pragma once

#include "Services/AbstractService.hpp"
#include "Common/Game/EntityContainer.hpp"

namespace Server
{
namespace Services
{

class EntityService : public Server::AbstractService<EntityService>
{
public:
    EntityService(Common::Game::EntityContainer & entities);
    void handle(const Common::Messages::EntityGetInfoReq &, Network::IConnection &);
    void handle(const Common::Messages::EntityChangeCourseReq &, Network::IConnection &);
    void handle(const Common::Messages::AbstractMessage &, Network::IConnection &) {}

private:
    Common::Game::EntityContainer & m_entities;
};

}
}
