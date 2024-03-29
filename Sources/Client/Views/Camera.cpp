#include "Cake/Diagnostics/Logger.hpp"
#include "Common/Game/Object/Ship.hpp"
#include "Common/Math/Degree.hpp"

#include "Camera.hpp"

using namespace Client::Views;

Camera::Camera(Graphics::IGraphics & graphics, Input::IInput & input, Common::Game::Player & player) :
    m_graphics(graphics),
    m_player(player),
    m_distance(5000),
    m_userOrientationChanging(false),
    m_userXAngle(0),
    m_userYAngle(0)
{
    input.addMouseListener(*this);

    m_cameraLight = graphics.getSceneManager().createLight("cameraLight");
    m_cameraLight->setType(Ogre::Light::LT_DIRECTIONAL);

    m_userXAngle.setSpeed(0.01);
    m_userYAngle.setSpeed(0.01);

    m_userXAngle.setDelay(50);
    m_userYAngle.setDelay(50);
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

    // basics
    auto cameraOrientation = m_graphics.toOgreQuaternion(orientation);
    auto cameraPosition = m_graphics.toOgreVector3(position);

    m_userXAngle.update();
    m_userYAngle.update();

    recalculateUserOrientation();

    // flip backwards
    cameraOrientation = cameraOrientation * Ogre::Quaternion(Ogre::Degree(180), Ogre::Vector3::UNIT_Y);

    // apply user orientation
    cameraOrientation = cameraOrientation * m_userOrientation;

    int angle = 0;
    Ogre::Quaternion cameraOrientationAngle(Ogre::Degree(-angle), Ogre::Vector3(1, 0, 0));
    auto cameraPositionDelta = cameraOrientationAngle * Ogre::Vector3(0, 0, *m_distance);

    cameraPosition += cameraOrientation * cameraPositionDelta;
    cameraOrientation = cameraOrientation * cameraOrientationAngle;

    Ogre::Camera & camera = m_graphics.getCamera();
    camera.setPosition(cameraPosition);
    camera.setOrientation(cameraOrientation);

    m_cameraLight->setDirection(cameraOrientation * -Ogre::Vector3::UNIT_Z);

    m_distance.update();
}

void Camera::zoomIn()
{
    m_distance += 2000;
}

void Camera::zoomOut()
{
    m_distance -= 2000;
}

Ogre::Vector3 Camera::getPosition() const
{
    return m_graphics.getCamera().getPosition();
}

Ogre::Quaternion Camera::getOrientation() const
{
    Ogre::Camera & camera = m_graphics.getCamera();
    return camera.getOrientation();
}

bool Camera::isUserOrientationChanging() const
{
    return m_userOrientationChanging;
}

void Camera::resetUserOrientation()
{
    m_userXAngle = 0;
    m_userYAngle = 0;
}

void Camera::mouseMoved(const OIS::MouseState & state)
{
    if (m_userOrientationChanging)
    {
        Common::Math::Degree newXAngle = *m_userXAngle + static_cast<float>(state.X.rel) * 0.5;
        Common::Math::Degree newYAngle = *m_userYAngle + static_cast<float>(state.Y.rel) * 0.5;

        newXAngle.normalize();
        newYAngle.normalize();

        m_userXAngle.set(*newXAngle);
        m_userYAngle.set(*newYAngle);

        recalculateUserOrientation();
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

void Camera::recalculateUserOrientation()
{
    m_userOrientation = 
        Ogre::Quaternion(Ogre::Degree(*m_userXAngle), Ogre::Vector3(0, 1, 0)) *
        Ogre::Quaternion(Ogre::Degree(*m_userYAngle), Ogre::Vector3(1, 0, 0));
}

