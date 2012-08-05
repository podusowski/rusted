#include <stdexcept>

#include "Cake/Diagnostics/Logger.hpp"
#include "Common/Game/Utilities/PasswordHash.hpp"
#include "Server/Game/PlayerContainer.hpp"

using namespace Server::Game;
using namespace Common;

PlayerContainer::PlayerContainer(DataBase::DataBase & db) :
    m_db(db)
{
}

int PlayerContainer::authorize(const std::string & login, 
                               const std::string & password, 
                               Network::IConnection & connection)
{
    auto it = m_connectionMap.find(&connection);

    try
    {
        if (it != m_connectionMap.end())
        {
            int id = checkCredentials(login, password);
            it->second->setId(id);
            it->second->setState(PLAYER_STATE_AUTHORIZED);
            return id;
        }
        else
        {
            throw std::runtime_error("no such connection");
        }
    }
    catch (...)
    {
        return false;
    }
}

void PlayerContainer::add(Network::IConnection & connection)
{
    LOG_DEBUG << "Adding player";

    boost::shared_ptr<Player> player(new Player());
    m_connectionMap.insert(std::make_pair(&connection, player));
}

void PlayerContainer::remove(Network::IConnection & connection)
{
    auto it = m_connectionMap.find(&connection);

    if (it != m_connectionMap.end())
    {
        LOG_DEBUG << "Removing player";

        m_connectionMap.erase(it);
    }
    else
    {
        throw std::runtime_error("connection doesnt exist");
    }
}

Player & PlayerContainer::getBy(Network::IConnection & connection)
{
    return *m_connectionMap.at(&connection);
}

std::vector<boost::shared_ptr<Player> > PlayerContainer::getAll(PlayerState state)
{
    std::vector<boost::shared_ptr<Player> > ret(m_connectionMap.size());

    for (auto it = m_connectionMap.begin(); it != m_connectionMap.end(); it++)
    {
        if (it->second->getState() == state)
        {
            ret.push_back(it->second); 
        }
    }

    return ret;
}

std::vector<Server::Network::IConnection *> PlayerContainer::getAllConnections(PlayerState state)
{
    std::vector<Server::Network::IConnection*> ret;

    for (auto it = m_connectionMap.begin(); it != m_connectionMap.end(); it++)
    {
        if (it->second->getState() == state)
        {
            ret.push_back(it->first); 
        }
    }

    return ret;
}

int PlayerContainer::checkCredentials(const std::string & login, const std::string & password)
{
    auto players = m_db.getRoot().getFirstChild("users").getChilds();

    for (auto it = players.begin(); it != players.end(); it++)
    {
        if ((*it)->getValue<std::string>("login") == login)
        {
            Common::Game::Utilities::PasswordHash hash;
            if ((*it)->getValue<std::string>("password") == hash.generate(password))
            {
                return (*it)->getValue<int>("id");
            }
            else
            {
                throw std::out_of_range("invalid password for player \"" + login + "\"");
            }
        }
    }
    throw std::out_of_range("no such player \"" + login + "\"");
}
