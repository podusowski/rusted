#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreParticleSystem.h>

#include "Cake/Diagnostics/Logger.hpp"
#include "Cake/Serialization/Tms.hpp"

#include "Common/Game/Object/Asteroid.hpp"
#include "VisualObject.hpp"

using namespace Client::Views;

VisualObject::VisualObject(
    Client::Graphics::IGraphics & graphics,
    Client::Input::IInput & input,
    Common::Game::Object::ObjectBase & object)
    :
    m_graphics(graphics),
    m_object(object)
{
    Ogre::SceneManager & scene = m_graphics.getSceneManager();

    std::string modelFilename = "Contents/objects/" + object.getModel();

    LOG_DEBUG << "Loading model: " << modelFilename;

    std::ifstream f(modelFilename);

    if (!f.good())
    {
        std::stringstream ss;
        ss << "can't read model file: " << modelFilename;
        throw std::runtime_error(ss.str());
    }

    Cake::Serialization::Tms tms(f);

    std::string mesh = tms.getValue<std::string>("mesh");
    m_entity = scene.createEntity(mesh);
    m_node = scene.getRootSceneNode()->createChildSceneNode();
    m_node->attachObject(m_entity);
    m_node->setScale(100.0, 100.0, 100.0);

    input.addObjectRightClickCallback(*m_entity, std::bind(&VisualObject::rightClickedCallback, this));

    try
    {
        for (int i = 1; i < 10; i++)
        {
            std::stringstream engineNameSs;
            engineNameSs << "engine";
            engineNameSs << i;

            int x = tms.getValue<int>(engineNameSs.str() + ".thrust.x");
            int y = tms.getValue<int>(engineNameSs.str() + ".thrust.y");
            int z = tms.getValue<int>(engineNameSs.str() + ".thrust.z");

            LOG_DEBUG << "Engine" << i << " thrust at: " << x << ", " << y << ", " << z;

            std::stringstream ss;
            ss << "engine-particle-" << object.getId() << "-" << i;
            Ogre::ParticleSystem * ps = scene.createParticleSystem(ss.str(), "EngineTail");

            auto * psNode = m_node->createChildSceneNode();
            psNode->setPosition(Ogre::Vector3(x, y, z));
            psNode->attachObject(ps);
        }
    }
    catch (const std::exception & ex)
    {
        LOG_ERR << "Can't create engine thrust effect, reason: " << ex.what();
    }
    
    update();
}

void VisualObject::setRightClickCallback(std::function<void()> callback)
{
    m_rightClickCallback = callback;
}

void VisualObject::setSelected(bool selected)
{
    m_node->showBoundingBox(selected);
}

Common::Game::Object::ObjectBase & VisualObject::getGameObject()
{
    return m_object;
}

void VisualObject::update()
{
    Common::Game::Position position = m_object.getPosition();
    auto orientation = m_object.getOrientation();

    m_node->setPosition(position.getX(), position.getY(), position.getZ());
    m_node->setOrientation(m_graphics.toOgreQuaternion(orientation));

    // apply Blender coordinations patch
    //m_node->roll(Ogre::Degree(90));
    //m_node->pitch(Ogre::Degree(90));
    m_node->yaw(Ogre::Degree(-90));
}

void VisualObject::rightClickedCallback()
{
    if (m_rightClickCallback)
        m_rightClickCallback();
}
