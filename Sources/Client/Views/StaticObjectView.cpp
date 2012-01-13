#include <boost/bind.hpp>

#include <OgreEntity.h>

#include "Cake/Diagnostics/Logger.hpp"

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

void StaticObjectView::staticObjectAdded(Common::Game::Object::StaticObject & object)
{
    LOG_DEBUG << "New static object:" << object;

    Ogre::SceneManager & scene = m_graphics.getSceneManager();

    Ogre::Entity * staticObjectMesh = scene.createEntity("Cube.mesh");
    Ogre::SceneNode * staticObjectNode = scene.getRootSceneNode()->createChildSceneNode();
//    staticObjectNode->scale(10, 10, 10);
    staticObjectNode->attachObject(staticObjectMesh);
    Common::Game::Position position = object.getPosition();
    staticObjectNode->setPosition(position.getX(), position.getY(), position.getZ());
}
