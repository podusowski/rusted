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

std::tuple<bool, Ogre::Vector2> Graphics::getScreenCoordinates(const Ogre::MovableObject & object)
{
    if (!object.isInScene())
    {
        return std::make_tuple(false, Ogre::Vector2());
    }
 
    const Ogre::AxisAlignedBox &AABB = object.getWorldBoundingBox(true);

    Ogre::Vector3 point = AABB.getCenter();
 
    // Is the camera facing that point? If not, return false
    Ogre::Plane cameraPlane = Ogre::Plane(Ogre::Vector3(m_ogreCamera->getDerivedOrientation().zAxis()), m_ogreCamera->getDerivedPosition());
    if (cameraPlane.getSide(point) != Ogre::Plane::NEGATIVE_SIDE)
    {
        return std::make_tuple(false, Ogre::Vector2());
    }
 
    // Transform the 3D point into screen space
    point = m_ogreCamera->getProjectionMatrix() * (m_ogreCamera->getViewMatrix() * point);

    Ogre::Vector2 result;
 
    // Transform from coordinate space [-1, 1] to [0, 1] and update in-value
    result.x = (point.x / 2) + 0.5f;
    result.y = 1 - ((point.y / 2) + 0.5f);

    result.x *= m_ogreRenderWindow->getViewport(0)->getActualWidth();
    result.y *= m_ogreRenderWindow->getViewport(0)->getActualHeight();
 
    return std::make_tuple(true, result);
}

void Graphics::initOgre()
{
    m_ogreRoot = new Ogre::Root("", "");

    m_ogreRenderSystem = new Ogre::GLRenderSystem();
    m_ogreRoot->setRenderSystem(m_ogreRenderSystem);

    m_particleFxPlugin = new Ogre::ParticleFXPlugin();
    m_ogreRoot->installPlugin(m_particleFxPlugin);

    m_ogreRoot->initialise(false);

    initResources();

    Ogre::NameValuePairList videoOptions;

    unsigned width = m_cfg->getValue<unsigned>("graphics.width", 800);
    unsigned height = m_cfg->getValue<unsigned>("graphics.height", 600);
    bool fullscreen = m_cfg->getValue<unsigned>("graphics.fullscreen", false);
    m_ogreRenderWindow = m_ogreRoot->createRenderWindow("Rusted", width, height, fullscreen, &videoOptions);

    m_ogreSceneManager = m_ogreRoot->createSceneManager(Ogre::ST_GENERIC, "SceneMgr");
    m_ogreSceneManager->setAmbientLight(Ogre::ColourValue(0, 0, 0));
    m_ogreCamera = m_ogreSceneManager->createCamera("SampleCam");

    Ogre::Viewport * vp = m_ogreRenderWindow->addViewport(m_ogreCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
    m_ogreCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void Graphics::initResources()
{
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Contents/particles", "FileSystem", "General");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Contents/meshes", "FileSystem", "General");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Contents/materials", "FileSystem", "General");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Contents/objects", "FileSystem", "General");
}

