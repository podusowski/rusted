#pragma once

#include "Common/Game/Player.hpp"
#include "Server/Network/IConnection.hpp"
#include "PlayerSummary.hpp"

namespace Server
{
namespace Game
{

class IPlayerContainer
{
public:
    virtual int authorize(const std::string & login,
                          const std::string & password, 
                          Network::IConnection & connection) = 0;

    virtual void add(Network::IConnection &) = 0;
    virtual void remove(Network::IConnection &) = 0;

    // TODO: this is not threadsafe and will be removed
    virtual Common::Game::IPlayer & getBy(Network::IConnection & connection) = 0;
    virtual Network::IConnection & getConnectionById(int) = 0;

    virtual void invokeOnPlayer(int, std::function<void(Common::Game::IPlayer &, Network::IConnection &)>) = 0;
    virtual void invokeOnAllPlayers(std::function<void(Common::Game::IPlayer &, Network::IConnection &)>) = 0;

    // TODO: threadsafe
    virtual std::vector<std::shared_ptr<Common::Game::Player> > getAll(Common::Game::PlayerState state) = 0;
    virtual std::vector<Network::IConnection *> getAllConnections(Common::Game::PlayerState state) = 0;

    virtual PlayerSummary getPlayerSummary(int id) = 0;

    virtual ~IPlayerContainer() {}
};

}
}
