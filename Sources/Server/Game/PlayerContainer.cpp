#include <stdexcept>

#include "Cake/Diagnostics/Logger.hpp"
#include "Cake/Threading/ScopedLock.hpp"

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
    LOG_DEBUG << "Connection: " << &connection << " wants to authorize as player: " << login;

    Cake::Threading::ScopedLock lock(m_mutex);

    auto it = m_connectionMap.find(&connection);

    if (it != m_connectionMap.end())
    {
        int id = checkCredentials(login, password);
        it->second->setId(id);
        it->second->setState(Common::Game::PLAYER_STATE_AUTHORIZED);
        return id;
    }
    else
    {
        throw std::runtime_error("no such connection");
    }
}

void PlayerContainer::add(Network::IConnection & connection)
{
    LOG_DEBUG << "Adding new connection: " << &connection << " and assiociating it with new player";

    boost::shared_ptr<Common::Game::Player> player(new Common::Game::Player());

    Cake::Threading::ScopedLock lock(m_mutex);
    m_connectionMap.insert(std::make_pair(&connection, player));
}

void PlayerContainer::remove(Network::IConnection & connection)
{
    Cake::Threading::ScopedLock lock(m_mutex);

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

Common::Game::Player & PlayerContainer::getBy(Network::IConnection & connection)
{
    // TODO: can't impl thread safety by this impl
    Cake::Threading::ScopedLock lock(m_mutex);
    return *m_connectionMap.at(&connection);
}

Server::Network::IConnection & PlayerContainer::getConnectionById(int playerId)
{
    // TODO: optimize this
    Cake::Threading::ScopedLock lock(m_mutex);
    for (auto & i: m_connectionMap)
    {
        if (i.second->getId() == playerId)
        {
            return *i.first;
        }
    }
    throw std::out_of_range("player doesn't exist");
}

void PlayerContainer::visitOnlinePlayerById(int id, std::function<void(Common::Game::Player &, Network::IConnection &)> visitor)
{
    //TODO: optimize
    Cake::Threading::ScopedLock lock(m_mutex);
    for (auto & i: m_connectionMap)
    {
        if (i.second->getId() == id)
        {
            visitor(*i.second, *i.first);
            return;
        }
    }
    throw std::out_of_range("player doesn't exist");
}

std::vector<boost::shared_ptr<Common::Game::Player> > PlayerContainer::getAll(Common::Game::PlayerState state)
{
    Cake::Threading::ScopedLock lock(m_mutex);

    std::vector<boost::shared_ptr<Common::Game::Player> > ret;

    for (auto it = m_connectionMap.begin(); it != m_connectionMap.end(); it++)
    {
        if (it->second->getState() == state)
        {
            ret.push_back(it->second); 
        }
    }

    return ret;
}

std::vector<Server::Network::IConnection *> PlayerContainer::getAllConnections(Common::Game::PlayerState state)
{
    Cake::Threading::ScopedLock lock(m_mutex);

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

PlayerSummary PlayerContainer::getPlayerSummary(int id)
{
    auto players = m_db.getRoot().getFirstChild("users").getChilds();

    for (auto it = players.begin(); it != players.end(); it++)
    {
        if ((*it)->getValue<int>("id") == id)
        {
            PlayerSummary ret;
            ret.id = id;
            ret.name = (*it)->getValue<std::string>("login");
            return ret;
        }
    }

    std::stringstream ss;
    ss << "no player with id: " << id;
    throw std::out_of_range(ss.str());
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

