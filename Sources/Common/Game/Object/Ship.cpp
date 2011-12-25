#include "Ship.hpp"

using namespace Common::Game::Object;
using namespace Common::Game;

Position Ship::getPosition()
{
    const unsigned speed = 1;

    unsigned distance = Position::distance(m_course.course, m_position);
    unsigned totalTripTime = distance / speed;
    unsigned timeTakenSoFar = m_time->getSeconds() - m_course.startTime;

    float tripProfress = timeTakenSoFar / totalTripTime;

    Position tripVector = m_course.course - m_position;

    return tripVector * tripProfress;
}

void Ship::setPosition(const Position &)
{
}

void Ship::setCourse(Position course)
{
    m_course.course = course;
    m_course.startTime = m_time->getSeconds();
}

unsigned Ship::getOwnerId()
{
}
