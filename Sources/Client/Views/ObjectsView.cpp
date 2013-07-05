#include <boost/bind.hpp>
#include <OgreEntity.h>

#include "Cake/Diagnostics/Logger.hpp"
#include "Common/Game/Object/Ship.hpp"
#include "Common/Game/Object/Asteroid.hpp"

#include "Graphics/IGraphics.hpp"

#include "Services/ObjectService.hpp"
#include "Services/PlayerActionService.hpp"

#include "Visuals/VisualUniverse.hpp"
#include "Visuals/VisualObject.hpp"

#include "Client/Views/ObjectsView.hpp"

using namespace Client::Views;

ObjectsView::ObjectsView(Services::ObjectService & objectService,
                         Services::PlayerActionService & playerActionService,
                         Graphics::IGraphics & graphics,
                         Effects::Effects & effects,
                         Input::IInput & input,
                         Gui::Gui & gui,
                         Common::Game::Universe & universe,
                         Visuals::VisualUniverse & visualUniverse) :
    m_objectService(objectService),
    m_playerActionService(playerActionService),
    m_graphics(graphics),
    m_effects(effects),
    m_input(input),
    m_gui(gui),
    m_universe(universe),
    m_visualUniverse(visualUniverse)
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
    m_visualUniverse.update();
    updateSelectedObjectWindow();
}

void ObjectsView::objectAdded(Common::Game::Object::ObjectBase & object)
{
    LOG_DEBUG << "New object:" << object;

    auto obj = m_visualUniverse.add(object);
    obj->setRightClickCallback(std::bind(&ObjectsView::objectClicked, this, obj.get()));
}

void ObjectsView::objectClicked(Visuals::VisualObject * object)
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

