#include "Object.hpp"

using namespace Client::Views;

Object::Object(Client::Graphics::IGraphics & graphics, Common::Game::Object::ObjectBase & object) :
    m_graphics(graphics),
    m_object(object)
{
    Ogre::SceneManager & scene = m_graphics.getSceneManager();

    Ogre::Entity * mesh = scene.createEntity("Cube.mesh");
    m_node = scene.getRootSceneNode()->createChildSceneNode();
    m_node->attachObject(mesh);
    
    update();
}

void Object::update()
{
    Common::Game::Position position = m_object.getPosition();
    m_node->setPosition(position.getX(), position.getY(), position.getZ());
}
