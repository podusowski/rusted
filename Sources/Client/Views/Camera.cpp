#include "Cake/Diagnostics/Logger.hpp"
#include "Common/Game/Object/Ship.hpp"

#include "Camera.hpp"

using namespace Client::Views;

Camera::Camera(Graphics::IGraphics & graphics, Common::Game::Player & player) :
    m_graphics(graphics),
    m_player(player)
{
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

    Ogre::Vector3 cameraPosition = m_graphics.toOgreVector3(position);
    cameraPosition += Ogre::Vector3(0, 0, 400);
    cameraPosition += cameraOrientation * Ogre::Vector3(0, -300, 0);

    cameraOrientation = cameraOrientation * Ogre::Quaternion(Ogre::Degree(40), Ogre::Vector3(1, 0, 0));

    Ogre::Camera & camera = m_graphics.getCamera();
    camera.setPosition(cameraPosition);
    camera.setOrientation(cameraOrientation);
}

