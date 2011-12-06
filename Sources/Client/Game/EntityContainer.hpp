#pragma once

#include <vector>

#include "Common/Game/Entity.hpp"
#include "Common/Game/IRustedTime.hpp"
#include "Client/Game/PlayerInfo.hpp"

namespace Client
{
namespace Game
{

class EntityContainer
{
public:
    EntityContainer(Common::Game::IRustedTime &, Client::Game::PlayerInfo &);
    ~EntityContainer();
    Common::Game::Entity & create(int id, int player, Common::Game::Entity::Position position);
    std::vector<Common::Game::Entity *> getMyEntities();
    Common::Game::Entity & getEntity(int id);

private:
    std::vector<Common::Game::Entity *> m_entities;
    Common::Game::IRustedTime & m_time;
    Client::Game::PlayerInfo & m_playerInfo;
};

}
}

