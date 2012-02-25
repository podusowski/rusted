#pragma once

#include <boost/scoped_ptr.hpp>

#include "Client/Views/IView.hpp"
#include "Client/Graphics/IGraphics.hpp"
#include "Client/Input/IInput.hpp"
#include "Gui/Gui.hpp" 
#include "Client/Graphics/OgreObject.hpp"
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
    void mousePressed(const OIS::MouseButtonID &, unsigned x, unsigned y);
    void mouseReleased(const OIS::MouseButtonID &, unsigned x, unsigned y);

private:
    Graphics::IGraphics & m_graphics;
    Input::IInput & m_input;
    Services::PlayerActionService & m_playerActionService;
    Services::ObjectService & m_objectService;
    Gui::Gui & m_gui;
};

}
}
