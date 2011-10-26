#pragma once

#include <map>
#include <string>

#include "Common/DataBase/DataBase.hpp"
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

    Player & create(const std::string & login,
                    const std::string & password, 
                    Network::IConnection & connection);

    Server::Game::Player & getBy(Network::IConnection & connection);

private:
    std::map<int, Server::Game::Player *> m_connectionMap;
    Common::DataBase::DataBase & m_db;
};

}
}

