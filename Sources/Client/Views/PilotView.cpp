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
    m_playerActionService.fetchAvailableActions(boost::bind(&PilotView::availableActionsFetched, this, _1));
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
    std::stringstream ss;
    ss << m_playerActionService.getFocusedObject().getPosition();
    m_gui->findWidget<MyGUI::TextBox>("NavigationTextBox")->setCaption(ss.str());
}

void PilotView::updateCameraPosition()
{
    // camera motion
    Common::Game::Position position = m_playerActionService.getFocusedObject().getPosition();
    Common::Game::Position camPosition = position + Common::Game::Position(0, 0, 150);

    // some nasty dbg 
    static int counter = 0;
    if (counter++ % 1000 == 0)
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

void PilotView::mousePressed(const OIS::MouseButtonID &, const OIS::MouseEvent &, unsigned x, unsigned y)
{
}

void PilotView::mouseReleased(const OIS::MouseButtonID & button, unsigned x, unsigned y)
{
    using namespace Common::Game;

    if (button == OIS::MB_Right)
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

void PilotView::actionClicked(MyGUI::Widget * widget)
{
    LOG_DEBUG << "Action clicked";

    int * actionId = widget->getUserData<int>();
    m_playerActionService.executeAction(*actionId);
}

void PilotView::availableActionsFetched(std::vector<boost::tuple<int, std::string> > actions)
{
    auto * actionsPanel = m_gui->findWidget<MyGUI::Widget>("ActionsPanel");

    LOG_DEBUG << "Got actions";
    for (auto & action: actions)
    {
        LOG_DEBUG << "  " << action.get<0>() << "/" << action.get<1>();

        auto * actionButton = actionsPanel->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(0, 0, 50, 50), MyGUI::Align::Default);
        actionButton->setCaption(action.get<1>());
        actionButton->setUserData(action.get<0>());
        actionButton->eventMouseButtonClick += MyGUI::newDelegate(this, &PilotView::actionClicked);
    }
}

