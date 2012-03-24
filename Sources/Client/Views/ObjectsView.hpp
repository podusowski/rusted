#pragma once

#include "Common/Game/Object/StaticObject.hpp"
#include "Client/Views/IView.hpp"
#include "Client/Services/ObjectService.hpp"
#include "Client/Services/PlayerActionService.hpp"
#include "Client/Graphics/IGraphics.hpp"
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
                Input::IInput &,
                Gui::Gui &,
                Common::Game::Universe &);

    void activate();
    void deactivate();
    void frameStarted();

    void objectAdded(Common::Game::Object::ObjectBase &);

private:
    void objectClicked(Object * object);
    void updateSelectedObjectWindow();

    Services::ObjectService & m_objectService;
    Services::PlayerActionService & m_playerActionService;
    Graphics::IGraphics & m_graphics;
    Input::IInput & m_input;
    Gui::Gui & m_gui;
    Common::Game::Universe & m_universe;
    std::vector<boost::shared_ptr<Object> > m_objects;
    boost::optional<Object *> m_selectedObject;
};

}
}
