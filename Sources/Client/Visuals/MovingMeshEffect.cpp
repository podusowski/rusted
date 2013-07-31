#include <OgreSceneNode.h>
#include <OgreEntity.h>

#include "Cake/Diagnostics/Logger.hpp"

#include "MovingMeshEffect.hpp"

using namespace Client::Visuals;

MovingMeshEffect::MovingMeshEffect(
    Graphics::IGraphics & graphics,
    Common::Game::Position start,
    Common::Game::Position end,
    int speed) : 
        m_graphics(graphics),
        m_speed(speed),
        m_start(start),
        m_end(end)
{
    LOG_DEBUG << "Creating MovingMeshEffect (from " << start << " to " << end << ")";

    m_startTime = m_time->getCurrentTime();

    Ogre::SceneManager & scene = m_graphics.getSceneManager();
    std::string mesh = "EnergyBall.mesh";
    m_entity = scene.createEntity(mesh);
    m_node = scene.getRootSceneNode()->createChildSceneNode();
    m_node->setScale(20.0, 20.0, 20.0);
    
    // to update position
    frameStarted();

    m_node->attachObject(m_entity);
}

MovingMeshEffect::~MovingMeshEffect()
{
    Ogre::SceneManager & scene = m_graphics.getSceneManager();
    scene.destroySceneNode(m_node);
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
    float totalTripTime = float(distance) / float(m_speed);
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

