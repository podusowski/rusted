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

void Ship::addAttack(boost::shared_ptr<Common::Game::Attack::IAttack> attack)
{
    LOG_DEBUG << "Adding attack: " << CAKE_DEPENDENCY_INJECTION_TYPENAME(*attack);
    m_attacks.push_back(attack);
}

void Ship::attack(unsigned attackId, ObjectBase & other)
{
    LOG_DEBUG << *this << " is attacking " << other << " with attack id:" << attackId;

    auto & attack = *m_attacks.at(attackId);
    attack.attack(other);
}

void Ship::addAction(boost::shared_ptr<Common::Game::Actions::IAction> action)
{
    LOG_DEBUG << "Adding IAction: " << CAKE_DEPENDENCY_INJECTION_TYPENAME(*action) << " with id:" << m_actions.size();

    m_actions.push_back(action);
}

void Ship::executeAction(unsigned id)
{
    Common::Game::Actions::IAction & action = *m_actions.at(id);

    LOG_DEBUG << "Executing IAction: " << CAKE_DEPENDENCY_INJECTION_TYPENAME(action) << " with id:" << id;

    action.execute();
}

void Ship::addActionOnAnotherObject(boost::shared_ptr<Common::Game::Actions::IActionOnAnotherObject> action)
{
    LOG_DEBUG << "Adding IActionOnAnotherObject: " << CAKE_DEPENDENCY_INJECTION_TYPENAME(*action) << " with id:" << m_actionsOnAnotherObject.size();

    m_actionsOnAnotherObject.push_back(action);
}

void Ship::executeActionOnAnotherObject(unsigned id, Common::Game::Object::ObjectBase & object)
{
    Common::Game::Actions::IActionOnAnotherObject & action = *m_actionsOnAnotherObject.at(id);

    LOG_DEBUG << "Executing IActionOnAnotherObject: " << CAKE_DEPENDENCY_INJECTION_TYPENAME(action) << " with id:" << id;

    action.execute(object);
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

