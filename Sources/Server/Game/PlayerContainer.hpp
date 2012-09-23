#pragma once

#include <map>
#include <string>

#include "Cake/Threading/Mutex.hpp"

#include "DataBase/DataBase.hpp"
#include "IPlayerContainer.hpp"

namespace Server
{
namespace Game
{

class PlayerContainer : public IPlayerContainer
{
public:
    PlayerContainer(Server::DataBase::DataBase & db);

    int authorize(const std::string & login,
                   const std::string & password, 
                   Network::IConnection & connection);

    void add(Network::IConnection &);
    void remove(Network::IConnection &);

    Server::Game::Player & getBy(Network::IConnection & connection);
    Network::IConnection & getConnectionById(int);
    
    std::vector<boost::shared_ptr<Player> > getAll(PlayerState state);
    std::vector<Network::IConnection *> getAllConnections(PlayerState state);

private:
    int checkCredentials(const std::string & login, const std::string & password);

    std::map<Network::IConnection *, boost::shared_ptr<Server::Game::Player> > m_connectionMap;
    Server::DataBase::DataBase & m_db;

    Cake::Threading::Mutex m_mutex;
};

}
}

