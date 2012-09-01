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
    // camera motion
    Common::Game::Position position = m_player.getFocusedObject().getPosition();
    Common::Game::Position camPosition = position + Common::Game::Position(0, 0, 1000);

    Ogre::Camera & camera = m_graphics.getCamera();
    camera.setPosition(camPosition.getX(), camPosition.getY(), camPosition.getZ());
    camera.lookAt(Ogre::Vector3(position.getX(), position.getY(), position.getZ()));
}

