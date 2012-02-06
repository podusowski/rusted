#include <stdexcept>
#include <boost/foreach.hpp>

#include "Cake/Diagnostics/Logger.hpp"
#include "Common/Game/Utilities/PasswordHash.hpp"
#include "Server/Game/PlayerContainer.hpp"

using namespace Server::Game;
using namespace Common;

PlayerContainer::PlayerContainer(DataBase::DataBase & db) :
    m_db(db)
{
}

Player & PlayerContainer::create(const std::string & login, 
                                 const std::string & password, 
                                 Network::IConnection & connection)
{
    DataBase::DataBaseNode & players = m_db.getRoot().getFirstChild("users");
    for (DataBase::DataBaseNode::iterator it = players.getChilds().begin();
         it != players.getChilds().end(); it++)
    {
        if ((*it)->getValue<std::string>("login") == login)
        {
            Common::Game::Utilities::PasswordHash hash;
            if ((*it)->getValue<std::string>("password") == hash.generate(password))
            {
                int id = (*it)->getValue<int>("id");
                Player * player = new Player(id);

                LOG_DEBUG << "Adding player: " << id << " with connection: " << connection.getId();

                m_connectionMap.insert(std::make_pair(&connection, player));
                return *player;
            }
            else
            {
                throw std::out_of_range("invalid password for player \"" + login + "\"");
            }
        }
    }
    throw std::out_of_range("no such player \"" + login + "\"");
}

Player & PlayerContainer::getBy(Network::IConnection & connection)
{
    return *m_connectionMap.at(&connection);
}

std::vector<Player*> PlayerContainer::getAll()
{
    std::vector<Player*> ret(m_connectionMap.size());

    for (std::map<Server::Network::IConnection *, Player*>::iterator it = m_connectionMap.begin();
         it != m_connectionMap.end(); it++)
    {
       ret.push_back(it->second); 
    }

    return ret;
}

std::vector<Server::Network::IConnection *> PlayerContainer::getAllConnections()
{
    std::vector<Server::Network::IConnection*> ret;

    for (std::map<Server::Network::IConnection *, Player *>::iterator it = m_connectionMap.begin();
         it != m_connectionMap.end(); it++)
    {
       ret.push_back(it->first); 
    }

    return ret;
}


