#include "MyGUI.h"

#include "Cake/Diagnostics/Logger.hpp"
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
    LOG_DEBUG << "Adding right click callback for &Entity: " << &entity;

    m_rightClickCallbacks.insert(std::make_pair(&entity, callback));
}

void OgreObjectRaycaster::mouseMoved(const OIS::MouseState &)
{
}

void OgreObjectRaycaster::mousePressed(const OIS::MouseButtonID &, const OIS::MouseEvent & arg, unsigned x, unsigned y)
{
    LOG_DEBUG << "Casting for clicked objects";

    auto mousePos = MyGUI::InputManager::getInstance().getMousePosition();
 
    Ogre::Ray mouseRay = m_camera.getCameraToViewportRay(
                            mousePos.left / float(arg.state.width), 
                            mousePos.top / float(arg.state.height));

    m_raySceneQuery->setRay(mouseRay);

    Ogre::RaySceneQueryResult & result = m_raySceneQuery->execute();
    Ogre::RaySceneQueryResult::iterator it = result.begin();

    if (it != result.end() && it->movable)
    {
        Ogre::Entity * entity = dynamic_cast<Ogre::Entity *>(it->movable);

        if (entity)
        {
            LOG_DEBUG << "Entity clicked (name: " << it->movable->getName() << ", &: " << entity << ")";

            auto rightClickCallbackIt = m_rightClickCallbacks.find(entity);

            if (rightClickCallbackIt != m_rightClickCallbacks.end())
            {
                LOG_DEBUG << "Found callback for it";

                rightClickCallbackIt->second();
            }
        }
    }
}

void OgreObjectRaycaster::mouseReleased(const OIS::MouseButtonID &, unsigned x, unsigned y)
{
}
