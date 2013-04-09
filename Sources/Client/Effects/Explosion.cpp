#include <sstream>

#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreParticleSystem.h>
#include <OgreParticleEmitter.h>

#include "Explosion.hpp"

using namespace Client::Effects;

int Explosion::m_id = 0;

Explosion::Explosion(Graphics::IGraphics & graphics, Common::Game::Position position) :
    m_graphics(graphics)
{
    auto & scene = graphics.getSceneManager();

    std::stringstream ss;
    ss << "explosion-particle-" << m_id++;
    m_ps = scene.createParticleSystem(ss.str(), "Explosion");

    auto * psNode = scene.getRootSceneNode()->createChildSceneNode();
    psNode->setPosition(Ogre::Vector3(position.getX(), position.getY(), position.getZ()));
    psNode->attachObject(m_ps);
}

void Explosion::frameStarted()
{
}

bool Explosion::isAlive()
{
    for (unsigned i = 0; i < m_ps->getNumEmitters(); i++)
    {
        Ogre::ParticleEmitter * emitter = m_ps->getEmitter(i);
        if (!emitter->getEnabled())
        {
            return false;
        }
    }
    return true;
}
