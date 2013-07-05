#pragma once

#include "Forwards.hpp"

#include "Views/IView.hpp"
#include "Client/Gui/Gui.hpp"

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
                Visuals::VisualUniverse &);

    void activate();
    void deactivate();
    void frameStarted();

    void objectAdded(Common::Game::Object::ObjectBase &);

private:
    void objectClicked(Visuals::VisualObject * object);
    void updateSelectedObjectWindow();

    Services::ObjectService & m_objectService;
    Services::PlayerActionService & m_playerActionService;
    Graphics::IGraphics & m_graphics;
    Effects::Effects & m_effects;
    Input::IInput & m_input;
    Gui::Gui & m_gui;
    Common::Game::Universe & m_universe;
    Visuals::VisualUniverse & m_visualUniverse;
    boost::optional<Visuals::VisualObject *> m_selectedObject;
};

}
}
