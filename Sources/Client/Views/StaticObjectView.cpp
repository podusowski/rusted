#include <boost/bind.hpp>
#include <OgreEntity.h>

#include "Cake/Diagnostics/Logger.hpp"
#include "Client/Views/StaticObjectView.hpp"

using namespace Client::Views;

StaticObjectView::StaticObjectView(Services::StaticObjectService & staticObjectService,
                                   Graphics::IGraphics & graphics,
                                   Common::Game::Universe & universe) :
    m_staticObjectService(staticObjectService),
    m_graphics(graphics),
    m_universe(universe)
{
}

void StaticObjectView::activate()
{
    m_universe.setObjectAddedCallback(boost::bind(&StaticObjectView::objectAdded, this, _1));
    m_staticObjectService.fetchStaticObjects();
}

void StaticObjectView::deactivate()
{
}

void StaticObjectView::frameStarted()
{
}

void StaticObjectView::objectAdded(Common::Game::Object::ObjectBase & object)
{
    if (typeid(object) == typeid(Common::Game::Object::StaticObject))
    {
        LOG_DEBUG << "New static object:" << object;

        Ogre::SceneManager & scene = m_graphics.getSceneManager();

        Ogre::Entity * staticObjectMesh = scene.createEntity("Cube.mesh");
        Ogre::SceneNode * staticObjectNode = scene.getRootSceneNode()->createChildSceneNode();
        staticObjectNode->attachObject(staticObjectMesh);
        Common::Game::Position position = object.getPosition();
        staticObjectNode->setPosition(position.getX(), position.getY(), position.getZ());
    }
}
