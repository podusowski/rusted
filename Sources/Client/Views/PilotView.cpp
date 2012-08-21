#include <OgreEntity.h>

#include "Cake/Diagnostics/Logger.hpp"
#include "Client/Views/PilotView.hpp"

using namespace Client::Views;

PilotView::PilotView(Graphics::IGraphics & graphics, 
                     Input::IInput & input, 
                     Services::PlayerActionService & playerActionService,
                     Services::ObjectService & objectService,
                     Client::Gui::Gui & gui,
                     Common::Game::Universe & universe,
                     Game::PlayerInfo & playerInfo) :
    m_graphics(graphics),
    m_input(input),
    m_playerActionService(playerActionService),
    m_objectService(objectService),
    m_gui(gui),
    m_universe(universe),
    m_playerInfo(playerInfo)
{
}

void PilotView::activate()
{
    m_input.addMouseListener(*this);
    m_universe.addObjectAddedCallback(boost::bind(&PilotView::objectAdded, this, _1));

    m_playerActionService.fetchAvailableActions(boost::bind(&PilotView::availableActionsFetched, this, _1));
    m_objectService.fetchPlayerShips(boost::bind(&PilotView::playerShipsFetched, this));

    MyGUI::ListBox * shipListBox = m_gui->findWidget<MyGUI::ListBox>("ShipListBox");
    shipListBox->eventListChangePosition += MyGUI::newDelegate(this, &PilotView::shipListBoxSelected);
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
    auto & focusedShip = dynamic_cast<Common::Game::Object::Ship&>(m_playerActionService.getFocusedObject());

    std::stringstream ss;
    ss << "crt: " << focusedShip.getPosition();

    Common::Game::Object::Course course = focusedShip.getCourse();

    if (focusedShip.getPosition() != course.destination)
    {
        ss << " | dst: " << course.destination;
    }

    m_gui->findWidget<MyGUI::TextBox>("NavigationTextBox")->setCaption(ss.str());
}

void PilotView::updateCameraPosition()
{
    // camera motion
    Common::Game::Position position = m_playerActionService.getFocusedObject().getPosition();
    Common::Game::Position camPosition = position + Common::Game::Position(0, 0, 1000);

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
    m_playerActionService.executeAction(*actionId, boost::bind(&PilotView::enableActionButtons, this));
    disableActionButtons();
}

void PilotView::disableActionButtons()
{
    for (auto * button: m_actionButtons)
    {
        button->setEnabled(false);
    }
}

void PilotView::enableActionButtons()
{
    for (auto * button: m_actionButtons)
    {
        button->setEnabled(true);
    }
}

void PilotView::availableActionsFetched(std::vector<boost::tuple<int, std::string> > actions)
{
    auto * actionsPanel = m_gui->findWidget<MyGUI::Widget>("ActionsPanel");

    LOG_DEBUG << "Got actions";

    int buttonTop = 0;
    for (auto & action: actions)
    {
        LOG_DEBUG << "  " << action.get<0>() << "/" << action.get<1>();

        auto * actionButton = actionsPanel->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(0, buttonTop, 50, 50), MyGUI::Align::Default);
        m_actionButtons.push_back(actionButton);
        buttonTop += 50;

        actionButton->setCaption(action.get<1>());
        actionButton->setUserData(action.get<0>());
        actionButton->eventMouseButtonClick += MyGUI::newDelegate(this, &PilotView::actionClicked);
    }
}

void PilotView::shipListBoxSelected(MyGUI::ListBox * listBox, size_t index)
{
    unsigned * id = listBox->getItemDataAt<unsigned>(index);

    LOG_DEBUG << "Changing focus to: " << *id;

    auto & ship = m_universe.getById<Common::Game::Object::Ship>(*id);
    m_playerActionService.focusObject(ship);
}

void PilotView::playerShipsFetched()
{
    LOG_DEBUG << "Got player ships";

    updatePlayerShipsListBox();
}

void PilotView::objectAdded(Common::Game::Object::ObjectBase & object)
{
    auto * ship = dynamic_cast<Common::Game::Object::Ship*>(&object);
    if (ship && ship->getOwnerId() == m_playerInfo.getId())
    {
        LOG_DEBUG << "Player ship added";
        updatePlayerShipsListBox();
    }
}

void PilotView::updatePlayerShipsListBox()
{
    MyGUI::ListBox * shipListBox = m_gui->findWidget<MyGUI::ListBox>("ShipListBox");

    shipListBox->removeAllItems();

    auto ships = m_universe.getByOwnerId<Common::Game::Object::Ship>(m_playerInfo.getId());
    for (auto ship: ships)
    {
        std::stringstream ss;
        ss << "Ship " << ship->getId();
        shipListBox->addItem(ss.str(), MyGUI::Any(ship->getId()));
    }
}

