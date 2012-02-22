#include <boost/bind.hpp>
#include <OgreEntity.h>

#include "Cake/Diagnostics/Logger.hpp"
#include "Common/Game/Object/Ship.hpp"
#include "Common/Game/Object/StaticObject.hpp"
#include "Client/Views/ObjectsView.hpp"

using namespace Client::Views;

ObjectsView::ObjectsView(Services::StaticObjectService & staticObjectService,
                         Services::ObjectService & objectService,
                         Graphics::IGraphics & graphics,
                         Common::Game::Universe & universe) :
    m_staticObjectService(staticObjectService),
    m_objectService(objectService),
    m_graphics(graphics),
    m_universe(universe)
{
}

void ObjectsView::activate()
{
    m_universe.setObjectAddedCallback(boost::bind(&ObjectsView::objectAdded, this, _1));
    m_objectService.fetchVisibleObjects();
}

void ObjectsView::deactivate()
{
}

void ObjectsView::frameStarted()
{
}

void ObjectsView::objectAdded(Common::Game::Object::ObjectBase & object)
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
    else if (typeid(object) == typeid(Common::Game::Object::Ship))
    {
        LOG_DEBUG << "New ship object:" << object;

        boost::shared_ptr<Object> obj(new Object(object));
        m_objects.push_back(obj);
    }
}
