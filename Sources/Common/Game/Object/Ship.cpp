#include "Cake/Diagnostics/Logger.hpp"

#include "Ship.hpp"

using namespace Common::Game::Object;
using namespace Common::Game;

Ship::Ship() : m_speed(1)
{
}

Position Ship::getPosition()
{
    return calculatePosition(m_time->getCurrentTime());
}

void Ship::setPosition(const Position & position)
{
    m_course.course = position;
    m_position = position;
}

void Ship::setCourse(Position position)
{
    TimeValue time = m_time->getCurrentTime();

    m_position = calculatePosition(time);
    m_course.course = position;
    m_course.startTime = time;

    LOG_DEBUG << "Setting course from " << m_position << " to " << position << ", start time: " << time << ", speed: " << m_speed;
}

void Ship::setSpeed(unsigned speed)
{
    m_speed = speed;
}

void Ship::setIntegrity(unsigned integrity)
{
    ObjectBase::setIntegrity(integrity);

    if (getIntegrity() == 0)
    {
        TimeValue time = m_time->getCurrentTime();
        m_position = calculatePosition(time);
        m_course.course = m_position;

        LOG_DEBUG << "Ship destroyed, leaving it at " << m_position;
    }
}

Position Ship::calculatePosition(TimeValue time)
{
    if (m_course.course == m_position)
        return m_position;

    unsigned distance = Position::distance(m_course.course, m_position);
    unsigned totalTripTime = distance / m_speed;
    TimeValue timeTakenSoFar = time - m_course.startTime;
    float secondsTakenSoFar = timeTakenSoFar.getSeconds() + (timeTakenSoFar.getMiliseconds() / 1000.0);


    if (timeTakenSoFar == TimeValue(0, 0))
        return m_position;

    float tripProgress = float(secondsTakenSoFar) / float(totalTripTime);

    if (tripProgress >= 1.0)
    {
        m_position = m_course.course;

        LOG_DEBUG << "Ship arrived at " << m_position;

        return m_position;
    }
    else
    {
        Position tripVector = m_course.course - m_position;
        return m_position + (tripVector * tripProgress);
    }
}

