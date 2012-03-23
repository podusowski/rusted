#include "Cake/Diagnostics/Logger.hpp"

#include "Object.hpp"

using namespace Client::Views;

Object::Object(Client::Graphics::IGraphics & graphics, Client::Input::IInput & input, Common::Game::Object::ObjectBase & object) :
    m_graphics(graphics),
    m_object(object)
{
    Ogre::SceneManager & scene = m_graphics.getSceneManager();

    Ogre::Entity * mesh = scene.createEntity("Cube.mesh");
    m_node = scene.getRootSceneNode()->createChildSceneNode();
    m_node->attachObject(mesh);

    input.addObjectRightClickCallback(*mesh, std::bind(&Object::rightClickedCallback, this));
    
    update();
}

void Object::setRightClickCallback(std::function<void()> callback)
{
    m_rightClickCallback = callback;
}

void Object::update()
{
    Common::Game::Position position = m_object.getPosition();
    m_node->setPosition(position.getX(), position.getY(), position.getZ());
}

void Object::rightClickedCallback()
{
    LOG_DEBUG << "Object clicked";
}
