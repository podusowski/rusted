#pragma once

#include "Server/Game/Player.hpp"
#include "Server/Network/IConnection.hpp"

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

    virtual Server::Game::Player & getBy(Network::IConnection & connection) = 0;
    virtual Network::IConnection & getConnectionById(int) = 0;
    virtual std::vector<boost::shared_ptr<Player> > getAll(PlayerState state) = 0;
    virtual std::vector<Network::IConnection *> getAllConnections(PlayerState state) = 0;

    virtual ~IPlayerContainer() {}
};

}
}
