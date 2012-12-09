#pragma once

#include "Common/Game/Universe.hpp"
#include "Server/Services/AbstractService.hpp"
#include "Server/Game/PlayerContainer.hpp"

namespace Server
{
namespace Services 
{

class PlayerService : public Server::AbstractService<PlayerService>
{
public:
    PlayerService(Common::Game::Universe &, Server::Game::PlayerContainer &);

    void handle(const Common::Messages::GetPlayerResourcesInfo &, Network::IConnection &);
    void handle(const Common::Messages::FetchPlayerShips &, Network::IConnection &);

    void handle(const Common::Messages::AbstractMessage &, Network::IConnection &) {}

private:
    Common::Game::Universe & m_universe;
    Server::Game::PlayerContainer & m_playerContainer;
};

}
}

