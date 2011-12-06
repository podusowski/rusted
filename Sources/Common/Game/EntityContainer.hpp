#ifndef SERVER_GAME_ENTITY_CONTAINER_HPP
#define SERVER_GAME_ENTITY_CONTAINER_HPP

#include "Common/Game/Entity.hpp"
#include "Common/DataBase/DataBase.hpp"
#include "Common/Game/IRustedTime.hpp"

namespace Common 
{
namespace Game
{

//TODO: this is server specific, client will have own implementation

/**
 * @brief All Entities in the world are stored and managed here.
 *
 * This class owns Entities, means that it is responsible for memory and id management.
 */
class EntityContainer
{
public:
    typedef std::vector<Entity*> Container;

    EntityContainer(Common::DataBase::DataBase &, Common::Game::IRustedTime &);
    ~EntityContainer();

    /**
     * Creates new entity.
     *
     * @param id Unique identifier for the entity FIXME!!!! why is called player?!
     * @return reference to newly created entity
     */
    Entity & create(int player);

    /**
     * Gets entity by its id.
     *
     * @param id of the entity
     * @return reference to the entity
     */
    Entity & getById(int id);

    /**
     * Gets collection of entities owned by given player.
     *
     * @param id player id
     * @return collection of entities
     */
    std::auto_ptr<Container> getByPlayerId(int id);

private:

    void loadFromDataBase();

    std::map<int, Entity *> m_entities;
    std::map<int, std::vector<Entity*>* > m_playerEntities;

    int m_lastId;
    Common::DataBase::DataBase & m_db;
    Common::Game::IRustedTime & m_time;
};

}
}

#endif
