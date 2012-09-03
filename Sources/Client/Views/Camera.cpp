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

    // move camera up
    Common::Game::Position cameraPosition = position + Common::Game::Position(0, 0, 500);

    Ogre::Camera & camera = m_graphics.getCamera();
    camera.setPosition(cameraPosition.getX(), cameraPosition.getY(), cameraPosition.getZ());

    camera.setOrientation(Ogre::Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));
    //camera.lookAt(Ogre::Vector3(position.getX(), position.getY(), position.getZ()));
}

