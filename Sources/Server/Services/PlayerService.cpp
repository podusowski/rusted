#include "Cake/Diagnostics/Logger.hpp"
#include "Common/Game/Object/Ship.hpp"

#include "Server/Services/PlayerService.hpp"

using namespace Server::Services;

PlayerService::PlayerService(Common::Game::Universe & universe, Server::Game::PlayerContainer & playerContainer) :
        m_universe(universe),
        m_playerContainer(playerContainer)
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

    Game::Player & player = m_playerContainer.getBy(connection);

    std::vector<boost::shared_ptr<Common::Game::Object::ObjectBase> > playerEntities
        = m_universe.getByOwnerId<Common::Game::Object::Ship>(player.getId());

    for (std::vector<boost::shared_ptr<Common::Game::Object::ObjectBase> >::iterator it = playerEntities.begin(); 
         it != playerEntities.end(); it++)
    {
        resp.entities.push_back(boost::make_tuple<int>((*it)->getId()));
    }

    connection.send(resp);
}
