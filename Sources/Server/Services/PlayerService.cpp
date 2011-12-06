#include "Common/Logger/Logger.hpp"
#include "Common/Game/Entity.hpp"

#include "Server/Services/PlayerService.hpp"

using namespace Server::Services;

PlayerService::PlayerService(Common::Game::EntityContainer & entityContainer) :
        m_entityContainer(entityContainer)
{
}

void PlayerService::handle(const Common::Messages::PlayerResourcesStatusReq &, Network::IConnection & connection)
{
    Common::Messages::PlayerResourcesStatusResp resp;
    resp.carbon = 0xf00d;
    resp.uranium = 0xf00d;
    resp.credits = 0xf00d;
    connection.send(resp);
}

void PlayerService::handle(const Common::Messages::PlayerEntitiesStatusReq &, Network::IConnection & connection)
{
    Common::Messages::PlayerEntitiesStatusResp resp;
#warning get proper player id when playerContainer is implemented
//TODO: get proper player id when playerContainer is implemented
    std::auto_ptr<Common::Game::EntityContainer::Container> playerEntities = m_entityContainer.getByPlayerId(1);
    for (Common::Game::EntityContainer::Container::iterator it = playerEntities->begin(); it != playerEntities->end(); it++)
    {
        resp.entities.push_back(boost::make_tuple<int>((*it)->getId()));
    }

    connection.send(resp);
}
