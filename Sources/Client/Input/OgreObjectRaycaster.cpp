#include "Client/Graphics/IGraphics.hpp"

#include "OgreObjectRaycaster.hpp"

using namespace Client::Input;

OgreObjectRaycaster::OgreObjectRaycaster()
{

}

void OgreObjectRaycaster::addObjectRightClickCallback(Ogre::Entity & entity, std::function<void()> callback)
{
    m_rightClickCallbacks.insert(std::make_pair(&entity, callback));
}

void OgreObjectRaycaster::mouseMoved(const OIS::MouseState &)
{
}

void OgreObjectRaycaster::mousePressed(const OIS::MouseButtonID &, unsigned x, unsigned y)
{
}

void OgreObjectRaycaster::mouseReleased(const OIS::MouseButtonID &, unsigned x, unsigned y)
{
}
