#include <sstream>

#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreParticleSystem.h>

#include "Explosion.hpp"

using namespace Client::Effects;

int Explosion::m_id = 0;

Explosion::Explosion(Graphics::IGraphics & graphics, Common::Game::Position position) :
    m_graphics(graphics)
{
    auto & scene = graphics.getSceneManager();

    std::stringstream ss;
    ss << "explosion-particle-" << m_id++;
    Ogre::ParticleSystem * ps = scene.createParticleSystem(ss.str(), "EngineTail");

    auto * psNode = scene.getRootSceneNode()->createChildSceneNode();
    psNode->setPosition(Ogre::Vector3(position.getX(), position.getY(), position.getZ()));
    psNode->attachObject(ps);
}

