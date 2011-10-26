#include "Common/Logger/Logger.hpp"
#include "Entity.hpp"

using namespace Common::Game;

Entity::Entity(Common::Game::IRustedTime & time, int id, int player, Position position) :
    m_time(time),
    m_id(id),
    m_playerId(player),
    m_position(position),
    m_destination(position),
    m_courseChangeTime(0)
{
}

int Entity::getId()
{
    return m_id;
}

int Entity::getPlayerId()
{
    return m_playerId;
}

Entity::Position Entity::getPosition()
{
    if (m_destination == m_position)
    {
        return m_position;
    }
    else
    {
        Common::Point3<int> tripVector = m_destination - m_position;
        int tripTime = Common::Point3<int>::distance(m_destination, m_position) / getSpeed();
        float tripProgress = float(m_time.getSeconds() - m_courseChangeTime) / float(tripTime);
        if (tripProgress > 1.0)
        {
            m_position = m_destination;
            return m_position;
        }
        else
        {
            return m_position + tripVector * tripProgress;
        }
    }
}

void Entity::setCourse(const Entity::Position & course)
{
    LOG_INFO << "Changing course (entityId: " << m_id << ", course: " << course << ", current position: " << getPosition() << ")\n";

    m_position = getPosition();
    m_courseChangeTime = m_time.getSeconds();
    m_destination = course;
}

unsigned Entity::getSpeed()
{
    return 1;
}

Entity::Position Entity::getDirection()
{
    return m_destination - m_position;
}
