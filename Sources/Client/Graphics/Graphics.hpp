#pragma once

#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreGLRenderSystem.h>
#include <OgreParticleFXPlugin.h>

#include "Client/Graphics//IGraphics.hpp"

#include "Cake/DependencyInjection/Inject.hpp"
#include "Cake/Configuration/Configuration.hpp"

namespace Client
{
namespace Graphics
{

class Graphics : public IGraphics
{
public:
    Graphics();
    ~Graphics();

    Ogre::Root & getOgreRoot();
    bool frameStarted();
    Ogre::SceneManager & getSceneManager();
    Ogre::RenderWindow & getRenderWindow();
    Ogre::Camera & getCamera();
    unsigned getWidth();
    unsigned getHeight();
    Ogre::Quaternion toOgreQuaternion(Common::Math::Quaternion);
    Ogre::Vector3 toOgreVector3(Common::Game::Position);
    Common::Game::Position toPosition(Ogre::Vector3);
    std::tuple<bool, Ogre::Vector2> getScreenCoordinates(const Ogre::MovableObject &);

private:
    void initOgre();
    void initResources();

    Cake::DependencyInjection::Inject<Cake::Configuration::Configuration> m_cfg;
    Ogre::Root * m_ogreRoot;
    Ogre::RenderSystem * m_ogreRenderSystem;
    Ogre::Plugin * m_particleFxPlugin;
    Ogre::RenderWindow * m_ogreRenderWindow;
    Ogre::SceneManager * m_ogreSceneManager;
    Ogre::Camera * m_ogreCamera;
};

}
}
