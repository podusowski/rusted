#include <boost/foreach.hpp>

#include "Common/Logger/Logger.hpp"
#include "Client/Game/EntityContainer.hpp"

using namespace Client::Game;

EntityContainer::EntityContainer(Common::Game::IRustedTime & time, 
                                 Client::Game::PlayerInfo & playerInfo) : 
    m_time(time),
    m_playerInfo(playerInfo)
{
}

EntityContainer::~EntityContainer()
{
    BOOST_FOREACH(Common::Game::Entity * entity, m_entities)
    {
        delete entity;
    }
}

Common::Game::Entity & EntityContainer::create(int id, 
                                               int player, 
                                               Common::Game::Entity::Position position)
{
    LOG_INFO << "New entity (id: " << id << ", player id: " << player << ")\n";
    Common::Game::Entity * entity = new Common::Game::Entity(m_time, id, player, position);
    m_entities.push_back(entity);
    return *entity;
}

std::vector<Common::Game::Entity *> EntityContainer::getMyEntities()
{
    LOG_INFO << "Get my entities list (player id: " << m_playerInfo.getId() << ")\n";
    std::vector<Common::Game::Entity *> myEntities;
    BOOST_FOREACH(Common::Game::Entity * entity, m_entities)
    {
        if (entity->getPlayerId() == m_playerInfo.getId())
        {
            myEntities.push_back(entity);
            LOG_INFO << "My Entity: " << entity->getId() << "\n";
        }
    }
    return myEntities;
}

Common::Game::Entity & EntityContainer::getEntity(int id)
{
    BOOST_FOREACH(Common::Game::Entity * entity, m_entities)
    {
        if (entity->getId() == id)
        {
            return *entity;
        }
    }
    throw std::out_of_range("no such entity");
}

