#include <OgreEntity.h>

#include "Cake/Diagnostics/Logger.hpp"
#include "Client/Views/PilotView.hpp"

using namespace Client::Views;

PilotView::PilotView(Graphics::IGraphics & graphics, 
                     Input::IInput & input, 
                     Services::PlayerActionService & playerActionService,
                     Services::ObjectService & objectService,
                     Client::Gui::Gui & gui) :
    m_graphics(graphics),
    m_input(input),
    m_playerActionService(playerActionService),
    m_objectService(objectService),
    m_gui(gui)
{
}

void PilotView::activate()
{
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
    CEGUI::Window * shipPosition = m_gui.getLayoutWindow().getChildRecursive("ShipPosition");
    std::stringstream ss;
    ss << m_playerActionService.getFocusedObject().getPosition();
    shipPosition->setText(ss.str());
}

void PilotView::updateCameraPosition()
{
    // camera motion
    Common::Game::Position position = m_playerActionService.getFocusedObject().getPosition();
    Common::Game::Position camPosition = position + Common::Game::Position(0, 0, 180);

    // some nasty dbg 
    static int counter = 0;
    if (counter++ % 100 == 0)
    {
        LOG_DEBUG << "camera posistion: " << camPosition;
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
        Ogre::Camera & camera = m_graphics.getCamera();
        const Ogre::Vector3 ogreCameraPosition = camera.getPosition();
        Position cameraPosition(ogreCameraPosition.x, ogreCameraPosition.y, 0);

        int top = y - (m_graphics.getHeight() / 2);
        int left = x - (m_graphics.getWidth() / 2);

        Position delta(left, -top, 0); 
        m_playerActionService.setFocusedObjectCourse(cameraPosition + delta);
    }
}
