#pragma once

#include "Client/Views/IView.hpp"
#include "Client/Graphics/IGraphics.hpp"
#include "Client/Input/IInput.hpp"
#include "Gui/Gui.hpp" 
#include "Client/Services/PlayerActionService.hpp"
#include "Client/Services/ObjectService.hpp"

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
              Client::Game::PlayerInfo &);

    ~PilotView();

    void activate();
    void deactivate();
    void frameStarted();

    void updateShipPosition();
    void updateCameraPosition();

    void mouseMoved(const OIS::MouseState &);
    void mousePressed(const OIS::MouseButtonID &, const OIS::MouseEvent &, unsigned x, unsigned y);
    void mouseReleased(const OIS::MouseButtonID &, unsigned x, unsigned y);

private:
    void actionClicked(MyGUI::Widget *);
    void disableActionButtons();
    void enableActionButtons();
    void availableActionsFetched(std::vector<boost::tuple<int, std::string> >);
    void shipListBoxSelected(MyGUI::ListBox *, size_t);
    void playerShipsFetched();
    void objectAdded(Common::Game::Object::ObjectBase &);
    void updatePlayerShipsListBox();

    Graphics::IGraphics & m_graphics;
    Input::IInput & m_input;
    Services::PlayerActionService & m_playerActionService;
    Services::ObjectService & m_objectService;
    Gui::Gui & m_gui;
    Common::Game::Universe & m_universe;
    Game::PlayerInfo & m_playerInfo;
    std::vector<MyGUI::Button *> m_actionButtons;
};

}
}
