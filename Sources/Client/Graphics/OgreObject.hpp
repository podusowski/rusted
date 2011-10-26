#pragma once

#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

namespace Client
{
namespace Graphics
{

class OgreObject
{
public:
    OgreObject(Ogre::SceneManager &, const std::string & mesh);
    Ogre::SceneNode & getSceneNode();
    Ogre::Entity & getEntity();

private:
    Ogre::SceneNode * m_node;
    Ogre::Entity * m_entity;

    static int m_lastId;
};

}
}
