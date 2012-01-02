#include <OgreEntity.h>
#include "Client/Views/PilotView.hpp"

using namespace Client::Views;

PilotView::PilotView(Graphics::IGraphics & graphics, 
                     Input::Input & input, 
                     Services::EntityService & entityService) :
    m_graphics(graphics),
    m_input(input),
    m_entityService(entityService)
{
}

void PilotView::activate()
{
    m_entityObject.reset(new Graphics::OgreObject(m_graphics.getSceneManager(), "Cube.mesh"));

    m_input.addMouseListener(*this);
}

void PilotView::deactivate()
{
}

void PilotView::frameStarted()
{
    updateShipPosition();
    updateCameraPosition();
}

void PilotView::updateShipPosition()
{
    Common::Game::Entity::Position position = m_entityService.getCurrentEntity().getPosition();
    m_entityObject->getSceneNode().setPosition(position.getX(), position.getY(), position.getZ());
}

void PilotView::updateCameraPosition()
{
    // camera motion
    Common::Game::Position position = m_entityService.getCurrentEntity().getPosition();
    Common::Game::Position camPosition = position + Common::Game::Position(0, 0, 150);

    // some nasty dbg 
    static int counter = 0;
    if (counter++ % 100 == 0)
    {
        LOG_INFO << "ship position(" << position << "), "
                 << "camera posistion(" << camPosition << ")\n";
    }

    Ogre::Camera & camera = m_graphics.getCamera();
    camera.setPosition(camPosition.getX(), camPosition.getY(), camPosition.getZ());
    camera.lookAt(Ogre::Vector3(position.getX(), position.getY(), position.getZ()));
}

void PilotView::mouseMoved(const OIS::MouseState & state)
{
}

void PilotView::mousePressed(const OIS::MouseButtonID &, unsigned x, unsigned y)
{
}

void PilotView::mouseReleased(const OIS::MouseButtonID & button, unsigned x, unsigned y)
{
    using namespace Common::Game;

    if (button == OIS::MB_Left)
    {
        Entity::Position position = m_entityService.getCurrentEntity().getPosition();

        int top = y - (m_graphics.getHeight() / 2);
        int left = x - (m_graphics.getWidth() / 2);

        Entity::Position delta(left, top, 0); 
        position += delta;
        m_entityService.setCourse(position);
    }
}
