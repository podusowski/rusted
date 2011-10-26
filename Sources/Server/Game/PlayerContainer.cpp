#include <stdexcept>
#include <boost/foreach.hpp>

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
                m_connectionMap.insert(std::make_pair(connection.getId(), player));
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
    return *m_connectionMap.at(connection.getId());
}


