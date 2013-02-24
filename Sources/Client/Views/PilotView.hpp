#pragma once

#include "Client/Views/IView.hpp"
#include "Client/Graphics/IGraphics.hpp"
#include "Client/Input/IInput.hpp"
#include "Gui/Gui.hpp" 
#include "Client/Services/PlayerActionService.hpp"
#include "Client/Services/ObjectService.hpp"
#include "Camera.hpp"

namespace Client
{
namespace Views
{

class PilotView : public IView, 
                  public Input::IMouseListener
{
public:
    PilotView(Graphics::IGraphics &, 
              Input::IInput &, 
              Services::PlayerActionService &,
              Services::ObjectService &,
              Gui::Gui &,
              Common::Game::Universe &,
              Common::Game::Player &);

    ~PilotView();

    void activate();
    void deactivate();
    void frameStarted();

    void updateShipPosition();

    void mouseMoved(const OIS::MouseState &);
    void mousePressed(const OIS::MouseButtonID &, const OIS::MouseEvent &, unsigned x, unsigned y);
    void mouseReleased(const OIS::MouseButtonID &, unsigned x, unsigned y);

private:
    void shipListBoxSelected(MyGUI::ListBox *, size_t);
    void playerShipsFetched();
    void objectAdded(Common::Game::Object::ObjectBase &);
    void updatePlayerShipsListBox();
    void updateFocusedShipWindow();

    Graphics::IGraphics & m_graphics;
    Input::IInput & m_input;
    Services::PlayerActionService & m_playerActionService;
    Services::ObjectService & m_objectService;
    Gui::Gui & m_gui;
    Common::Game::Universe & m_universe;
    Common::Game::Player & m_player;
    Camera m_camera;
};

}
}
