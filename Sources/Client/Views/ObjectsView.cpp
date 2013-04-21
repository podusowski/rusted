#include <boost/bind.hpp>
#include <OgreEntity.h>

#include "Cake/Diagnostics/Logger.hpp"
#include "Common/Game/Object/Ship.hpp"
#include "Common/Game/Object/Asteroid.hpp"

#include "Client/Views/ObjectsView.hpp"

using namespace Client::Views;

ObjectsView::ObjectsView(Services::ObjectService & objectService,
                         Services::PlayerActionService & playerActionService,
                         Graphics::IGraphics & graphics,
                         Effects::Effects & effects,
                         Input::IInput & input,
                         Gui::Gui & gui,
                         Common::Game::Universe & universe,
                         VisualObjectContainer & visualObjectContainer) :
    m_objectService(objectService),
    m_playerActionService(playerActionService),
    m_graphics(graphics),
    m_effects(effects),
    m_input(input),
    m_gui(gui),
    m_universe(universe),
    m_visualObjectContainer(visualObjectContainer)
{
}

void ObjectsView::activate()
{
    auto objects = m_universe.getAll();
    for (auto object: objects)
    {
        objectAdded(*object);
    }

    m_universe.addObjectAddedCallback(boost::bind(&ObjectsView::objectAdded, this, _1));
    m_objectService.fetchVisibleObjects();

    m_playerActionService.addObjectAttackedSlot(boost::bind(&ObjectsView::objectAttacked, this, _1, _2));

    Ogre::Light* pointLight = m_graphics.getSceneManager().createLight("pointLight");
    pointLight->setType(Ogre::Light::LT_POINT);
    pointLight->setPosition(Ogre::Vector3(0, 150, 250));
    pointLight->setPowerScale(20.0);
}

void ObjectsView::deactivate()
{
}

void ObjectsView::frameStarted()
{
    m_visualObjectContainer.update();
    updateSelectedObjectWindow();
}

void ObjectsView::objectAdded(Common::Game::Object::ObjectBase & object)
{
    LOG_DEBUG << "New object:" << object;

    auto obj = m_visualObjectContainer.add(object);
    obj->setRightClickCallback(std::bind(&ObjectsView::objectClicked, this, obj.get()));
}

void ObjectsView::objectAttacked(unsigned attackerId, unsigned attackedId)
{
}

void ObjectsView::objectClicked(VisualObject * object)
{
    LOG_DEBUG << "Object selected, id:" << object->getGameObject().getId();

    if (m_selectedObject)
    {
        (*m_selectedObject)->setSelected(false);
    }

    m_selectedObject = object;
    object->setSelected(true);
    m_playerActionService.selectObject(object->getGameObject());
}

void ObjectsView::updateSelectedObjectWindow()
{
    if (m_selectedObject)
    {
        m_gui->findWidget<MyGUI::TextBox>("TargetTextBox")->setCaption((*m_selectedObject)->getString());
    }
}

