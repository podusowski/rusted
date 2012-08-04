#include "Cake/Diagnostics/Logger.hpp"

#include "Common/Game/Actions/ISimpleAction.hpp"
#include "Common/Game/Actions/IActionOnAnotherObject.hpp"

#include "Ship.hpp"

using namespace Common::Game::Object;
using namespace Common::Game;

Ship::Ship() : m_speed(1), m_selectedObject(nullptr)
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

void Ship::addAction(boost::shared_ptr<Common::Game::Actions::IAction> action)
{
    LOG_DEBUG << "Adding IAction: " << CAKE_DEPENDENCY_INJECTION_TYPENAME(*action) << " with id:" << m_actions.size();

    m_actions.push_back(action);
}

void Ship::executeAction(unsigned id)
{
    Common::Game::Actions::IAction & action = *m_actions.at(id);

    LOG_DEBUG << "Executing IAction: " << CAKE_DEPENDENCY_INJECTION_TYPENAME(action) << " with id:" << id;

    Common::Game::Actions::ISimpleAction * simpleAction = dynamic_cast<Common::Game::Actions::ISimpleAction*>(&action);
    Common::Game::Actions::IActionOnAnotherObject * actionOnAnotherObject = dynamic_cast<Common::Game::Actions::IActionOnAnotherObject*>(&action);

    if (simpleAction)
    {
        simpleAction->execute();
    }
    else if (actionOnAnotherObject)
    {
        if (m_selectedObject)
        {
            actionOnAnotherObject->execute(*m_selectedObject);
        }
        else
        {
            throw std::runtime_error("no selected object while trying to execute IActionOnAnotherObject");
        }
    }
    else
    {
        throw std::runtime_error("unkown type of action");
    }
}

void Ship::selectObject(Common::Game::Object::ObjectBase & object)
{
    m_selectedObject = &object;
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

