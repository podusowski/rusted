#include "Cake/Diagnostics/Logger.hpp"
#include "Common/Game/Object/Ship.hpp"

#include "Camera.hpp"

using namespace Client::Views;

Camera::Camera(Graphics::IGraphics & graphics, Input::IInput & input, Common::Game::Player & player) :
    m_graphics(graphics),
    m_player(player),
    m_distance(1000),
    m_userXAngle(0),
    m_userYAngle(0),
    m_userOrientationChanging(false)
{
    input.addMouseListener(*this);
}

Camera::~Camera()
{
}

void Camera::update()
{
    auto & ship = dynamic_cast<Common::Game::Object::Ship&>(m_player.getFocusedObject());

    // camera motion
    Common::Game::Position position = ship.getPosition();

    auto orientation = ship.getOrientation();
    auto cameraOrientation = m_graphics.toOgreQuaternion(orientation);

    // apply user orientation
    cameraOrientation = cameraOrientation * m_userOrientation;

    int angle = 20;

    auto cameraPosition = m_graphics.toOgreVector3(position);
    auto cameraPositionDelta = Ogre::Quaternion(Ogre::Degree(-angle), Ogre::Vector3(1, 0, 0)) * Ogre::Vector3(0, -m_distance, 0);
    cameraPosition += cameraOrientation * cameraPositionDelta;

    cameraOrientation = cameraOrientation * Ogre::Quaternion(Ogre::Degree(90 - angle), Ogre::Vector3(1, 0, 0));

    Ogre::Camera & camera = m_graphics.getCamera();
    camera.setPosition(cameraPosition);
    camera.setOrientation(cameraOrientation);
}

void Camera::zoomIn()
{
    m_distance += 100;
}

void Camera::zoomOut()
{
    m_distance -= 100;
}

void Camera::mouseMoved(const OIS::MouseState & state)
{
    if (m_userOrientationChanging)
    {
        m_userXAngle += state.X.rel;
        m_userYAngle += state.Y.rel;

        m_userOrientation = 
            Ogre::Quaternion(Ogre::Degree(m_userXAngle), Ogre::Vector3(0, 0, 1)) *
            Ogre::Quaternion(Ogre::Degree(m_userYAngle), Ogre::Vector3(1, 0, 0));
    }
}

void Camera::mousePressed(const OIS::MouseButtonID & button, const OIS::MouseEvent &, unsigned x, unsigned y)
{
    if (button == OIS::MB_Middle)
    {
        m_userOrientationChanging = true;
    }
}

void Camera::mouseReleased(const OIS::MouseButtonID & button, unsigned x, unsigned y)
{
    if (button == OIS::MB_Middle)
    {
        m_userOrientationChanging = false;
    }
}

