#pragma once

#include <map>
#include <string>

#include <soci.h>

#include "Cake/Threading/Mutex.hpp"
#include "Cake/DependencyInjection/Inject.hpp"

#include "IPlayerContainer.hpp"

namespace Server
{
namespace Game
{

class PlayerContainer : public IPlayerContainer
{
public:
    PlayerContainer();

    int authorize(const std::string & login,
                   const std::string & password, 
                   Network::IConnection & connection);

    void add(Network::IConnection &);
    void remove(Network::IConnection &);

    Network::IConnection & getConnectionById(int);

    void invokeOnPlayer(int, std::function<void(Common::Game::IPlayer &, Network::IConnection &)>);
    void invokeOnPlayer(Network::IConnection &, std::function<void(Common::Game::IPlayer &)>) override;
    void invokeOnAllPlayers(std::function<void(Common::Game::IPlayer &, Network::IConnection &)>);

    std::vector<std::shared_ptr<Common::Game::Player> > getAll(Common::Game::PlayerState state);
    std::vector<Network::IConnection *> getAllConnections(Common::Game::PlayerState state);

    PlayerSummary getPlayerSummary(int id);

private:
    int checkCredentials(const std::string & login, const std::string & password);

    std::map<Network::IConnection *, std::shared_ptr<Common::Game::Player> > m_connectionMap;

    Cake::Threading::Mutex m_mutex;

    Cake::DependencyInjection::Inject<soci::session> m_sociSession;
};

}
}

