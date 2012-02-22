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
    auto objects = m_universe.getAll();
    for (auto object: objects)
    {
        objectAdded(*object);
    }

    m_universe.setObjectAddedCallback(boost::bind(&ObjectsView::objectAdded, this, _1));
    m_objectService.fetchVisibleObjects();
}

void ObjectsView::deactivate()
{
}

void ObjectsView::frameStarted()
{
    for (auto object: m_objects)
    {
        object->update();
    }
}

void ObjectsView::objectAdded(Common::Game::Object::ObjectBase & object)
{
    LOG_DEBUG << "New object:" << object;

    boost::shared_ptr<Object> obj(new Object(m_graphics, object));
    m_objects.push_back(obj);
}
