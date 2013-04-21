#pragma once

#include "Views/IView.hpp"
#include "Services/ObjectService.hpp"
#include "Services/PlayerActionService.hpp"
#include "Graphics/IGraphics.hpp"
#include "Effects/Effects.hpp"
#include "Client/Gui/Gui.hpp"
#include "VisualObject.hpp"
#include "VisualObjectContainer.hpp"

namespace Client
{
namespace Views
{

class ObjectsView : public IView
{
public:
    ObjectsView(Services::ObjectService &,
                Services::PlayerActionService &,
                Graphics::IGraphics &,
                Effects::Effects &,
                Input::IInput &,
                Gui::Gui &,
                Common::Game::Universe &,
                VisualObjectContainer &);

    void activate();
    void deactivate();
    void frameStarted();

    void objectAdded(Common::Game::Object::ObjectBase &);
    void objectAttacked(unsigned attacker, unsigned attacked);

private:
    void objectClicked(VisualObject * object);
    void updateSelectedObjectWindow();

    Services::ObjectService & m_objectService;
    Services::PlayerActionService & m_playerActionService;
    Graphics::IGraphics & m_graphics;
    Effects::Effects & m_effects;
    Input::IInput & m_input;
    Gui::Gui & m_gui;
    Common::Game::Universe & m_universe;
    VisualObjectContainer & m_visualObjectContainer;
    boost::optional<VisualObject *> m_selectedObject;
};

}
}
