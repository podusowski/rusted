#pragma once

#include <OgreRoot.h>
#include <OgreRenderWindow.h>

#include "Client/Graphics//IGraphics.hpp"

namespace Client
{
namespace Graphics
{

class Graphics : public IGraphics
{
public:
    Graphics();
    Ogre::Root & getOgreRoot();
    Ogre::SceneManager & getSceneManager();
    Ogre::RenderWindow & getRenderWindow();
    Ogre::Camera & getCamera();
    unsigned getWidth();
    unsigned getHeight();
    Ogre::Quaternion toOgreQuaternion(Common::Math::Quaternion);
    Ogre::Vector3 toOgreVector3(Common::Game::Position);

private:
    void initOgre();
    void initResources();

    Ogre::Root * m_ogreRoot;
    Ogre::RenderWindow * m_ogreRenderWindow;
    Ogre::SceneManager * m_ogreSceneManager;
    Ogre::Camera * m_ogreCamera;
};

}
}
