#include <OgreSceneNode.h>
#include <OgreEntity.h>

#include "Cake/Diagnostics/Logger.hpp"

#include "Common/Game/Object/StaticObject.hpp"
#include "Object.hpp"

using namespace Client::Views;

Object::Object(Client::Graphics::IGraphics & graphics, Client::Input::IInput & input, Common::Game::Object::ObjectBase & object) :
    m_graphics(graphics),
    m_object(object)
{
    Ogre::SceneManager & scene = m_graphics.getSceneManager();

    std::string mesh = "Cube.mesh";

    if (object.is<Common::Game::Object::StaticObject>())
    {
        mesh = "Asteroid.mesh";
    }

    m_entity = scene.createEntity(mesh);
    m_node = scene.getRootSceneNode()->createChildSceneNode();
    m_node->attachObject(m_entity);
    m_node->setScale(10.0, 10.0, 10.0);

    // apply Blender coordinations patch
    m_node->roll(Ogre::Degree(90));
    m_node->pitch(Ogre::Degree(90));

    input.addObjectRightClickCallback(*m_entity, std::bind(&Object::rightClickedCallback, this));
    
    update();
}

void Object::setRightClickCallback(std::function<void()> callback)
{
    m_rightClickCallback = callback;
}

void Object::setSelected(bool selected)
{
    m_node->showBoundingBox(selected);
}

Common::Game::Object::ObjectBase & Object::getGameObject()
{
    return m_object;
}

void Object::update()
{
    Common::Game::Position position = m_object.getPosition();
    m_node->setPosition(position.getX(), position.getY(), position.getZ());

    auto orientation = m_object.getOrientation();
LOG_DEBUG << orientation;
    m_node->setOrientation(m_graphics.toOgreQuaternion(orientation));
    //m_node->setOrientation(Ogre::Quaternion(Ogre::Radian(1), Ogre::Vector3(0, 0, 1)));
}

void Object::rightClickedCallback()
{
    if (m_rightClickCallback)
        m_rightClickCallback();
}
