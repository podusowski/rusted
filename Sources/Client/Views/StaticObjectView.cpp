#include <boost/bind.hpp>

#include <OgreEntity.h>

#include "Common/Logger/Logger.hpp"
#include "Client/Views/StaticObjectView.hpp"

using namespace Client::Views;

StaticObjectView::StaticObjectView(Services::StaticObjectService & staticObjectService,
                                   Graphics::IGraphics & graphics) :
    m_staticObjectService(staticObjectService),
    m_graphics(graphics)
{
}

void StaticObjectView::activate()
{
    m_staticObjectService.asyncFetchStaticObjects(boost::bind(&StaticObjectView::staticObjectAdded, this, _1));
}

void StaticObjectView::deactivate()
{
}

void StaticObjectView::frameStarted()
{
}

void StaticObjectView::staticObjectAdded(Common::Game::StaticObject & object)
{
    LOG_INFO << "New static object (id:" << object.getId() << ")\n";

    Ogre::SceneManager & scene = m_graphics.getSceneManager();

    Ogre::Entity * staticObjectMesh = scene.createEntity("staticObject", "Cube.mesh");
    Ogre::SceneNode * staticObjectNode = scene.getRootSceneNode()->createChildSceneNode();
    staticObjectNode->scale(10, 10, 10);
    staticObjectNode->attachObject(staticObjectMesh);
    Common::Game::StaticObject::Position position = object.getPosition();
    staticObjectNode->setPosition(position.getX(), position.getY(), position.getZ());
}
