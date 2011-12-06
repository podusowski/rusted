#include <boost/lexical_cast.hpp>

#include "Client/Graphics/OgreObject.hpp"

using namespace Client::Graphics;

int OgreObject::m_lastId;

OgreObject::OgreObject(Ogre::SceneManager & sceneManager, const std::string & mesh)
{
    std::string token = "OgreObject." + boost::lexical_cast<std::string>(m_lastId++);
    m_entity = sceneManager.createEntity(token + ".Entity", mesh);
    m_node = sceneManager.getRootSceneNode()->createChildSceneNode();
    m_node->attachObject(m_entity);
}

Ogre::SceneNode & OgreObject::getSceneNode()
{
    return *m_node;
}

Ogre::Entity & OgreObject::getEntity()
{
    return *m_entity;
}
