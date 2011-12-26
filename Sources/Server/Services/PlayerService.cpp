#include "Common/Logger/Logger.hpp"
#include "Common/Game/Entity.hpp"
#include "Common/Game/Object/Ship.hpp"

#include "Server/Services/PlayerService.hpp"

using namespace Server::Services;

PlayerService::PlayerService(Common::Game::Universe & universe) :
        m_universe(universe)
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
//
    std::vector<boost::shared_ptr<Common::Game::Object::ObjectBase> > playerEntities
        = m_universe.getByOwnerId<Common::Game::Object::Ship>(1);

    for (std::vector<boost::shared_ptr<Common::Game::Object::ObjectBase> >::iterator it = playerEntities.begin(); 
         it != playerEntities.end(); it++)
    {
        resp.entities.push_back(boost::make_tuple<int>((*it)->getId()));
    }

    connection.send(resp);
}
