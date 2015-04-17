#include "Cake/Diagnostics/Logger.hpp"
#include "Common/Game/Object/Ship.hpp"

#include "Server/Services/PlayerService.hpp"

using namespace Server::Services;

PlayerService::PlayerService(Common::Game::Universe & universe, Server::Game::PlayerContainer & playerContainer) :
        m_universe(universe),
        m_playerContainer(playerContainer)
{
}

void PlayerService::handle(const Common::Messages::GetPlayerResourcesInfo &, Network::IConnection & connection)
{
    m_playerContainer.invokeOnPlayer(connection, [&] (Common::Game::IPlayer & player)
    {
        Common::Messages::PlayerResourcesInfo resp;
        resp.carbon = 0xf00d;
        resp.helium = 0xf00d;
        connection.send(resp);
    });
}

void PlayerService::handle(const Common::Messages::FetchPlayerShips &, Network::IConnection & connection)
{
    Common::Messages::PlayerShips resp;

    m_playerContainer.invokeOnPlayer(connection, [&] (Common::Game::IPlayer & player)
    {
        auto playerShips = m_universe.getByOwnerId<Common::Game::Object::Ship>(player.getId());
        for (auto it = playerShips.begin(); it != playerShips.end(); it++)
        {
            Common::Messages::ShipId shipId;
            shipId.id = (*it)->getId();

            resp.ships.push_back(shipId);
        }

        connection.send(resp);
    });
}
