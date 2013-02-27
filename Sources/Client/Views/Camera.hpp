#pragma once

#include "Common/Game/Player.hpp"
#include "Client/Input/IInput.hpp"
#include "Client/Graphics/IGraphics.hpp"

namespace Client
{
namespace Views
{

class Camera : public Input::IMouseListener
{
public:
    Camera(Graphics::IGraphics &, Input::IInput &, Common::Game::Player &);
    ~Camera();

    void update();
    void zoomIn();
    void zoomOut();
    Ogre::Quaternion getOrientation();
    bool isUserOrientationChanging();

    void mouseMoved(const OIS::MouseState &);
    void mousePressed(const OIS::MouseButtonID &, const OIS::MouseEvent &, unsigned x, unsigned y);
    void mouseReleased(const OIS::MouseButtonID &, unsigned x, unsigned y);

private:
    Graphics::IGraphics & m_graphics;
    Common::Game::Player & m_player;
    int m_distance;
    Ogre::Quaternion m_userOrientation;
    int m_userXAngle;
    int m_userYAngle;
    bool m_userOrientationChanging;
};

}
}
