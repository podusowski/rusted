#pragma once

#include <boost/scoped_ptr.hpp>

#include "Client/Views/IView.hpp"
#include "Client/Graphics/IGraphics.hpp"
#include "Client/Input/IInput.hpp"
#include "Gui/Gui.hpp" 
#include "Client/Graphics/OgreObject.hpp"
#include "Client/Services/EntityService.hpp"

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
              Services::EntityService &,
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
    Services::EntityService & m_entityService;
    Gui::Gui & m_gui;
    boost::scoped_ptr<Graphics::OgreObject> m_entityObject;
    boost::scoped_ptr<Graphics::OgreObject> m_navigatorPlaneObject;
};

}
}
