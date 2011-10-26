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
    m_entityObject->getSceneNode().scale(20, 20, 20);

    m_navigatorPlaneObject.reset(new Graphics::OgreObject(m_graphics.getSceneManager(), "Cylinder.001.mesh"));
    //m_navigatorPlaneObject->getSceneNode().scale(200, .5, 200);

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
    m_navigatorPlaneObject->getSceneNode().setPosition(position.getX(), position.getY(), position.getZ());
}

void PilotView::updateCameraPosition()
{
    using Common::Game::Entity;

    // camera motion
    Entity::Position position = m_entityService.getCurrentEntity().getPosition();
    Common::Point3<float> direction(m_entityService.getCurrentEntity().getDirection());
    direction = direction.normalize();
    Entity::Position camPosition = position - Entity::Position( direction * 800);

    // some nasty dbg 
    static int counter = 0;
    if (counter++ % 100 == 0)
    {
        LOG_INFO << "entity (" << position << "), "
                 << "cam pos (" << camPosition << "), "
                 << "direction ( " << direction << ")\n";
    }

    Ogre::Camera & camera = m_graphics.getCamera();
    camera.setPosition(camPosition.getX(), camPosition.getY(), camPosition.getZ());
    camera.lookAt(Ogre::Vector3(position.getX(), position.getY(), position.getZ()));
}

void PilotView::mouseMoved(const OIS::MouseState & state)
{
    if (state.buttonDown(OIS::MB_Right))
    {
        m_navigatorPlaneObject->getSceneNode().pitch(Ogre::Degree(state.Y.rel / 2.0));
    }
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
        // TODO: add navigatorplane's pitch to the vector

        int forward = y - (m_graphics.getHeight() / 2);
        int left = x - (m_graphics.getWidth() / 2);

        Entity::Position delta(left, 0, -forward); 
        position += delta;
        m_entityService.setCourse(position);
    }
}
