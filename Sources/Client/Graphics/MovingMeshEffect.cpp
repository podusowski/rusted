#include <OgreSceneNode.h>
#include <OgreEntity.h>

#include "Cake/Diagnostics/Logger.hpp"

#include "MovingMeshEffect.hpp"

using namespace Client::Graphics;

MovingMeshEffect::MovingMeshEffect(IGraphics & graphics,
    Common::Game::Position start,
    Common::Game::Position end) : 
        m_graphics(graphics),
        m_start(start),
        m_end(end)
{
    LOG_DEBUG << "Creating MovingMeshEffect";

    Ogre::SceneManager & scene = m_graphics.getSceneManager();
    std::string mesh = "Cube.mesh";

    m_entity = scene.createEntity(mesh);
    m_node = scene.getRootSceneNode()->createChildSceneNode();
    m_node->attachObject(m_entity);
}

void MovingMeshEffect::frameStarted()
{
}

bool MovingMeshEffect::isAlive()
{
    return false;
}

