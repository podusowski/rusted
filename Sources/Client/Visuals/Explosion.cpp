#include <sstream>

#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreParticleSystem.h>
#include <OgreParticleEmitter.h>

#include "Graphics/Raycast.hpp"
#include "Explosion.hpp"

using namespace Client::Visuals;

int Explosion::m_id = 0;

Explosion::Explosion(Graphics::IGraphics & graphics, VisualObject & object, Common::Game::Position direction) :
    m_graphics(graphics),
    m_object(object),
    m_direction(direction)
{
    auto & scene = graphics.getSceneManager();

    auto objectPosition = object.getOgreSceneNode().getPosition();
    auto origin = objectPosition - (graphics.toOgreVector3(m_direction) * 50);
    auto ogreDirection = graphics.toOgreVector3(m_direction).normalisedCopy();

    LOG_DEBUG << "Origin for raycast: " << origin << " (object pos: " << objectPosition << ", direction: " << m_direction << ")";

    Ogre::Ray ray(origin, ogreDirection);

    Graphics::Raycast raycast(graphics.getSceneManager());

    Graphics::RaycastResult result;

    raycast.cast(ray, [&](Ogre::Entity * entity, Ogre::Vector3 position) -> bool
    {
        if (entity == &object.getOgreEntity())
        {
            result.valid = true;
            result.entity = entity;
            result.position = position;

            return false;
        }
        else
        {
            return true;
        }
    });

    if (!result.valid)
    {
        throw std::runtime_error("cant raycast to determinate explosion position");
    }

    std::stringstream ss;
    ss << "explosion-particle-" << m_id++;
    m_ps = scene.createParticleSystem(ss.str(), "Explosion");

    // FIXME: hax caused by VisualObject which scales ogre node by 100
    auto explosionPositionDelta = (object.getOgreSceneNode().getPosition() - result.position) / 100;

    LOG_DEBUG << "Creating explosion, object pos: " << object.getOgreSceneNode().getPosition() << ", explosion pos: " << explosionPositionDelta;

    auto * psNode = object.getOgreSceneNode().createChildSceneNode();
    psNode->setPosition(explosionPositionDelta);
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

