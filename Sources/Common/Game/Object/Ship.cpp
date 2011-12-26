#include "Ship.hpp"

using namespace Common::Game::Object;
using namespace Common::Game;

Position Ship::getPosition()
{
    const unsigned speed = 1;

    if (m_course.course == m_position)
        return m_position;

    unsigned distance = Position::distance(m_course.course, m_position);
    unsigned totalTripTime = distance / speed;
    unsigned timeTakenSoFar = m_time->getSeconds() - m_course.startTime;

    float tripProgress = timeTakenSoFar / totalTripTime;

    if (tripProgress >= 1.0)
    {
        m_position = m_course.course;
        return m_position;
    }
    else
    {
        Position tripVector = m_course.course - m_position;
        return tripVector * tripProgress;
    }
}

void Ship::setPosition(const Position & position)
{
    m_course.course = position;
    m_position = position;
}

void Ship::setCourse(Position course)
{
    m_course.course = course;
    m_course.startTime = m_time->getSeconds();
}

