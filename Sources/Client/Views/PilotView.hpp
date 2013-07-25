#pragma once

#include "Common/Forwards.hpp"
#include "Forwards.hpp"

#include "Gui/Gui.hpp"
#include "Client/Views/IView.hpp"
#include "Client/Input/IInput.hpp"
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
              Common::Game::Player &,
              Visuals::VisualUniverse &);

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
    void flyToTargetButtonClicked(MyGUI::WidgetPtr);
    void playerShipsFetched();
    void objectAdded(Common::Game::Object::ObjectBase &);
    void updatePlayerShipsListBox();
    void updateFocusedShipWindow();
    void createOrientationPlane();
    void updateOrientationPlane();

    void createDestinationMarker();
    void updateDestinationMarker();

    Graphics::IGraphics & m_graphics;
    Input::IInput & m_input;
    Services::PlayerActionService & m_playerActionService;
    Services::ObjectService & m_objectService;
    Gui::Gui & m_gui;
    Common::Game::Universe & m_universe;
    Common::Game::Player & m_player;
    Visuals::VisualUniverse & m_visualUniverse;
    Camera m_camera;
    Ogre::SceneNode * m_orientationPlaneSceneNode;

    Ogre::Entity * m_courseMarkerEntity;
    Ogre::SceneNode * m_courseMarkerNode;
};

}
}
