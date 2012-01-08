#include "Cake/Diagnostics/Logger.hpp"

#include "Ship.hpp"

using namespace Common::Game::Object;
using namespace Common::Game;

Position Ship::getPosition()
{
    return calculatePosition(m_time->getSeconds());
}

void Ship::setPosition(const Position & position)
{
    m_course.course = position;
    m_position = position;
}

void Ship::setCourse(Position position)
{
    unsigned time = m_time->getSeconds();

    m_position = calculatePosition(time);
    m_course.course = position;
    m_course.startTime = time;

    LOG_DEBUG << "Setting course from " << m_position << " to " << position << ", start time: " << time << "\n";
}

Position Ship::calculatePosition(unsigned time)
{
    const unsigned speed = 1;

    if (m_course.course == m_position)
        return m_position;

    unsigned distance = Position::distance(m_course.course, m_position);
    unsigned totalTripTime = distance / speed;
    unsigned timeTakenSoFar = time - m_course.startTime;

    if (timeTakenSoFar == 0)
        return m_position;

    float tripProgress = float(timeTakenSoFar) / float(totalTripTime);

    if (tripProgress >= 1.0)
    {
        m_position = m_course.course;

        LOG_DEBUG << "Ship arrived at " << m_position << "\n";

        return m_position;
    }
    else
    {
        Position tripVector = m_course.course - m_position;
        return m_position + (tripVector * tripProgress);
    }
}

