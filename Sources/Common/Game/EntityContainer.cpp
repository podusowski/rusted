#include <iostream>
#include <stdexcept>

#include "Common/Game/EntityContainer.hpp"
#include "Common/Logger/Logger.hpp"

using namespace Common::Game;

EntityContainer::EntityContainer(DataBase::DataBase & db, Common::Game::IRustedTime & time) : 
    m_lastId(100), 
    m_db(db),
    m_time(time)
{
    try
    {
        m_db.getRoot().getFirstChild("entities");
        loadFromDataBase();
    }
    catch (std::out_of_range ex)
    {
        LOG_WARN << "Can't load entities from db, reason: " << ex.what() << ", empty node will be created\n";
        m_db.getRoot().createChild("entities");
    }
}

EntityContainer::~EntityContainer()
{
    for (std::map<int, std::vector<Entity*>* >::iterator it = m_playerEntities.begin();
            it != m_playerEntities.end(); it++)
    {
        delete it->second;
    }

    for (std::map<int, Entity *>::iterator it = m_entities.begin(); it != m_entities.end(); it++)
    {
        delete it->second;
    }
}

Entity & EntityContainer::create(int player)
{
    //FIXME: this will colide with db values
    int id = ++m_lastId;

    DataBase::DataBaseNode & node = m_db.getRoot().getFirstChild("entities").createChild(boost::lexical_cast<std::string>(id));
    node.setValue("id", id);

    Entity * entity = new Entity(m_time, id, player, Entity::Position());
    m_entities.insert(std::pair<int, Entity*>(id, entity));

    std::map<int, std::vector<Entity*>* >::iterator it = m_playerEntities.find(player);
    if (it != m_playerEntities.end())
    {
        it->second->push_back(entity);
    }
    else
    {
        std::vector<Entity*> * entities = new std::vector<Entity*>();
        entities->push_back(entity);
        m_playerEntities.insert(std::pair<int, std::vector<Entity*>*>(player, entities));
    }

    return *entity;
}

Entity & EntityContainer::getById(int id)
{
    if (m_entities.find(id) == m_entities.end())
    {
        LOG_ERR << "No such entity\n";
        throw std::out_of_range("no such entity");
    }

    return *m_entities.at(id);
}

std::auto_ptr<EntityContainer::Container> EntityContainer::getByPlayerId(int id)
{
    std::auto_ptr<EntityContainer::Container> container(new EntityContainer::Container);

    std::map<int, std::vector<Entity*>* >::iterator it = m_playerEntities.find(id);

    if (it != m_playerEntities.end())
    {
        *container = *it->second;
    }

    return container;
}

void EntityContainer::loadFromDataBase()
{
    DataBase::DataBaseNode & node = m_db.getRoot().getFirstChild("entities");

    LOG_INFO << "Loading entities from db\n";

    for (DataBase::DataBaseNode::iterator it = node.getChilds().begin();
         it != node.getChilds().end(); it++)
    {
        DataBase::DataBaseNode & entityNode = **it;
        Entity * entity = new Entity(m_time, 
                                     entityNode.getValue<int>("id"), 
                                     entityNode.getValue<int>("player_id"),
                                     Entity::Position(
                                         entityNode.getValue<int>("x"),
                                         entityNode.getValue<int>("y"),
                                         entityNode.getValue<int>("z")
                                     ));

        LOG_INFO << "   Loading entity from db, id: " << (*it)->getValue<int>("id") << "\n";

        m_entities.insert(std::make_pair(entityNode.getValue<int>("id"), entity));

        int player = entityNode.getValue<int>("player_id");

        std::map<int, std::vector<Entity*>* >::iterator it = m_playerEntities.find(player);
        if (it != m_playerEntities.end())
        {
            it->second->push_back(entity);
        }
        else
        {
            std::vector<Entity*> * entities = new std::vector<Entity*>();
            entities->push_back(entity);
            m_playerEntities.insert(std::pair<int, std::vector<Entity*>*>(player, entities));
        }
    }
}

