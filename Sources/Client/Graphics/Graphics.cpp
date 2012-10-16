#include <OgreWindowEventUtilities.h>

#include "Cake/Diagnostics/Logger.hpp"
#include "Client/Graphics/Graphics.hpp"

using namespace Client::Graphics;

Graphics::Graphics()
{
    initOgre();
    initResources();
}

Graphics::~Graphics()
{
}

Ogre::Root & Graphics::getOgreRoot()
{
    return *m_ogreRoot;
}

bool Graphics::frameStarted()
{
    getRenderWindow().swapBuffers(true);
    Ogre::WindowEventUtilities::messagePump();

    return m_ogreRoot->renderOneFrame();
}

Ogre::SceneManager & Graphics::getSceneManager()
{
    return *m_ogreSceneManager;
}

Ogre::RenderWindow & Graphics::getRenderWindow()
{
    return *m_ogreRenderWindow;
}

Ogre::Camera & Graphics::getCamera()
{
    return *m_ogreCamera;
}

unsigned Graphics::getWidth()
{
    return m_ogreRenderWindow->getWidth();
}

unsigned Graphics::getHeight()
{
    return m_ogreRenderWindow->getHeight();
}

Ogre::Quaternion Graphics::toOgreQuaternion(Common::Math::Quaternion q)
{
    return Ogre::Quaternion(q.getW(), q.getX(), q.getY(), q.getZ());
}

Ogre::Vector3 Graphics::toOgreVector3(Common::Game::Position position)
{
    return Ogre::Vector3(position.getX(), position.getY(), position.getZ());
}

Common::Game::Position Graphics::toPosition(Ogre::Vector3 vector)
{
    return Common::Game::Position(vector.x, vector.y, vector.z);
}

void Graphics::initOgre()
{
    m_ogreRoot = new Ogre::Root("", "");

    m_ogreRoot->loadPlugin("RenderSystem_GL");

    if (m_ogreRoot->getAvailableRenderers().begin() == m_ogreRoot->getAvailableRenderers().end())
    {
        LOG_ERR << "No OGRE renderers available";
        throw std::runtime_error("Ogre init error");
    }

    m_ogreRoot->setRenderSystem(*m_ogreRoot->getAvailableRenderers().begin());
    m_ogreRoot->initialise(false);

    initResources();

    Ogre::NameValuePairList videoOptions;
    m_ogreRenderWindow = m_ogreRoot->createRenderWindow("Rusted", 800, 600, false, &videoOptions);

    Ogre::SceneManager * sm = m_ogreRoot->createSceneManager(Ogre::ST_GENERIC, "SceneMgr");
    m_ogreSceneManager = sm;

    m_ogreSceneManager->setAmbientLight(Ogre::ColourValue(255, 255, 255));
    Ogre::Camera * camera = sm->createCamera("SampleCam");
    m_ogreCamera = camera;

    Ogre::Viewport * vp = m_ogreRenderWindow->addViewport(camera);
    vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
    camera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}

void Graphics::initResources()
{
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Contents/meshes", "FileSystem", "General");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Contents/materials", "FileSystem", "General");
}
