#include <OgreSceneNode.h>
#include <OgreEntity.h>

#include "Cake/Diagnostics/Logger.hpp"

#include "MovingMeshEffect.hpp"

using namespace Client::Graphics;

MovingMeshEffect::MovingMeshEffect(IGraphics & graphics,
    Common::Game::Position start,
    Common::Game::Position end) : 
        m_graphics(graphics),
        m_speed(10),
        m_start(start),
        m_end(end)
{
    LOG_DEBUG << "Creating MovingMeshEffect";

    Ogre::SceneManager & scene = m_graphics.getSceneManager();
    std::string mesh = "Cube.mesh";

    m_entity = scene.createEntity(mesh);
    m_node = scene.getRootSceneNode()->createChildSceneNode();
    m_node->attachObject(m_entity);

    m_startTime = m_time->getCurrentTime();
}

void MovingMeshEffect::frameStarted()
{
    auto now = m_time->getCurrentTime();
    auto position = calculatePosition(now);
    m_node->setPosition(position.getX(), position.getY(), position.getZ());
}

bool MovingMeshEffect::isAlive()
{
    return m_end != m_start;
}

Common::Game::Position MovingMeshEffect::calculatePosition(Common::Game::TimeValue time)
{
    if (m_end == m_start)
        return m_start;

    unsigned distance = Common::Game::Position::distance(m_end, m_start);
    unsigned totalTripTime = distance / m_speed;
    auto timeTakenSoFar = time - m_startTime;
    float secondsTakenSoFar = timeTakenSoFar.getSeconds() + (timeTakenSoFar.getMiliseconds() / 1000.0);

    if (timeTakenSoFar == Common::Game::TimeValue(0, 0))
        return m_start;

    float progress = float(secondsTakenSoFar) / float(totalTripTime);

    if (progress >= 1.0)
    {
        m_start = m_end;

        LOG_DEBUG << "Effect finished";

        return m_end;
    }
    else
    {
        auto trajectoryVector = m_end - m_start;
        return m_start + (trajectoryVector * progress);
    }
}

