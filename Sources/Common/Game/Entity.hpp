#pragma once

#include "Common/Point3.hpp"
#include "Common/Game/IRustedTime.hpp"

namespace Common 
{
namespace Game
{

/**
 * \brief Basic information about entity in space.
 *
 * Its not wrapped for now, we can represent by this only something that
 * can fly somewhere. Eventually name of this class should change to Ship or
 * something similar.
 */
class Entity
{
public:
    typedef Common::Point3<int> Position;

    Entity(Common::Game::IRustedTime & time, int id, int player, Position position);
    int getId();
    int getPlayerId();
    Position getPosition();
    void setCourse(const Position & course);
    unsigned getSpeed();
    Position getDirection();

private:
    Common::Game::IRustedTime & m_time;
    int m_id;
    int m_playerId;
    Common::Point3<int> m_position;
    Common::Point3<int> m_destination;
    unsigned m_courseChangeTime;
};

}
}

