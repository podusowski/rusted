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
              Gui::Gui &);

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

    Graphics::IGraphics & m_graphics;
    Input::IInput & m_input;
    Services::PlayerActionService & m_playerActionService;
    Services::ObjectService & m_objectService;
    Gui::Gui & m_gui;
};

}
}
