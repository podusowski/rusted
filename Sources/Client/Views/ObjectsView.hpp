#pragma once

#include "Common/Game/Object/StaticObject.hpp"

#include "Views/IView.hpp"
#include "Services/ObjectService.hpp"
#include "Services/PlayerActionService.hpp"
#include "Graphics/IGraphics.hpp"
#include "Graphics/Effects.hpp"
#include "Client/Gui/Gui.hpp"
#include "Object.hpp"

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
                Graphics::Effects &,
                Input::IInput &,
                Gui::Gui &,
                Common::Game::Universe &);

    void activate();
    void deactivate();
    void frameStarted();

    void objectAdded(Common::Game::Object::ObjectBase &);
    void objectAttacked(unsigned attacker, unsigned attacked);

private:
    void objectClicked(Object * object);
    void updateSelectedObjectWindow();

    Services::ObjectService & m_objectService;
    Services::PlayerActionService & m_playerActionService;
    Graphics::IGraphics & m_graphics;
    Graphics::Effects & m_effects;
    Input::IInput & m_input;
    Gui::Gui & m_gui;
    Common::Game::Universe & m_universe;
    std::vector<boost::shared_ptr<Object> > m_objects;
    boost::optional<Object *> m_selectedObject;
};

}
}
