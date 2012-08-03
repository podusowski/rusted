#pragma once

#include <map>
#include <string>

#include "DataBase/DataBase.hpp"
#include "Server/Game/Player.hpp"
#include "Server/Network/IConnection.hpp"

namespace Server
{
namespace Game
{

class PlayerContainer
{
public:
    PlayerContainer(Common::DataBase::DataBase & db);

    int authorize(const std::string & login,
                   const std::string & password, 
                   Network::IConnection & connection);

    void add(Network::IConnection &);
    void remove(Network::IConnection &);

    Server::Game::Player & getBy(Network::IConnection & connection);
    std::vector<boost::shared_ptr<Player> > getAll(PlayerState state);
    std::vector<Network::IConnection *> getAllConnections(PlayerState state);

private:
    int checkCredentials(const std::string & login, const std::string & password);

    std::map<Network::IConnection *, boost::shared_ptr<Server::Game::Player> > m_connectionMap;
    Common::DataBase::DataBase & m_db;
};

}
}

