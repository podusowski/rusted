#pragma once

#include <map>
#include <string>

#include <soci.h>

#include "Cake/Threading/Mutex.hpp"
#include "Cake/DependencyInjection/Inject.hpp"

#include "DataBase/DataBase.hpp"
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

    Common::Game::Player & getBy(Network::IConnection & connection);
    Network::IConnection & getConnectionById(int);

    void visitOnlinePlayerById(int, std::function<void(Common::Game::Player &, Network::IConnection &)>);

    std::vector<boost::shared_ptr<Common::Game::Player> > getAll(Common::Game::PlayerState state);
    std::vector<Network::IConnection *> getAllConnections(Common::Game::PlayerState state);

    PlayerSummary getPlayerSummary(int id);

private:
    int checkCredentials(const std::string & login, const std::string & password);

    std::map<Network::IConnection *, boost::shared_ptr<Common::Game::Player> > m_connectionMap;

    Cake::Threading::Mutex m_mutex;

    Cake::DependencyInjection::Inject<soci::session> m_sociSession;
};

}
}

