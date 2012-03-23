#include "Cake/Diagnostics/Logger.hpp"
#include "Gui/CEGUIIncludes.hpp"
#include "Client/Graphics/IGraphics.hpp"

#include "OgreObjectRaycaster.hpp"

using namespace Client::Input;

OgreObjectRaycaster::OgreObjectRaycaster(Ogre::SceneManager & ogreSceneManager, Ogre::Camera & camera) :
    m_camera(camera)
{
    m_raySceneQuery = ogreSceneManager.createRayQuery(Ogre::Ray());
}

void OgreObjectRaycaster::addObjectRightClickCallback(Ogre::Entity & entity, std::function<void()> callback)
{
    m_rightClickCallbacks.insert(std::make_pair(&entity, callback));
}

void OgreObjectRaycaster::mouseMoved(const OIS::MouseState &)
{
}

void OgreObjectRaycaster::mousePressed(const OIS::MouseButtonID &, const OIS::MouseEvent & arg, unsigned x, unsigned y)
{
    LOG_DEBUG << "Casting for clicked objects";

    CEGUI::Point mousePos = CEGUI::MouseCursor::getSingleton().getPosition();
 
    Ogre::Ray mouseRay = m_camera.getCameraToViewportRay(
                            mousePos.d_x / float(arg.state.width), 
                            mousePos.d_y / float(arg.state.height));

    m_raySceneQuery->setRay(mouseRay);

    Ogre::RaySceneQueryResult & result = m_raySceneQuery->execute();
    Ogre::RaySceneQueryResult::iterator it = result.begin();

    if (it != result.end() && it->movable)
    {
        LOG_DEBUG << "Object clicked";
    }
}

void OgreObjectRaycaster::mouseReleased(const OIS::MouseButtonID &, unsigned x, unsigned y)
{
}
