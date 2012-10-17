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
                     Common::Game::Player & player) :
    m_graphics(graphics),
    m_input(input),
    m_playerActionService(playerActionService),
    m_objectService(objectService),
    m_gui(gui),
    m_universe(universe),
    m_player(player),
    m_camera(graphics, player)
{
}

PilotView::~PilotView()
{
}

void PilotView::activate()
{
    m_input.addMouseListener(*this);
    m_universe.addObjectAddedCallback(boost::bind(&PilotView::objectAdded, this, _1));

    m_playerActionService.addGlobalCooldownExpiredSlot(boost::bind(&PilotView::enableActionButtons, this));
    m_playerActionService.addAvailableActionsFetchedSlot(boost::bind(&PilotView::availableActionsFetched, this, _1));
    m_playerActionService.fetchAvailableActions();

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
    m_camera.update();
}

void PilotView::updateShipPosition()
{
    auto & focusedShip = dynamic_cast<Common::Game::Object::Ship&>(m_player.getFocusedObject());

    std::stringstream ss;
    ss << "crt: " << focusedShip.getPosition();

    auto description = focusedShip.getTrajectoryDescription();

    if (focusedShip.getPosition() != description.destination)
    {
        ss << " | dst: " << description.destination;
    }

    m_gui->findWidget<MyGUI::TextBox>("NavigationTextBox")->setCaption(ss.str());
}

void PilotView::mouseMoved(const OIS::MouseState & state)
{
    if (state.Z.rel < 0)
    {
        m_camera.zoomIn();
    }
    else if (state.Z.rel > 0)
    {
        m_camera.zoomOut();
    }
}

void PilotView::mousePressed(const OIS::MouseButtonID &, const OIS::MouseEvent &, unsigned x, unsigned y)
{
}

void PilotView::mouseReleased(const OIS::MouseButtonID & button, unsigned x, unsigned y)
{
    using namespace Common::Game;

    if (button == OIS::MB_Right)
    {
        auto position = m_player.getFocusedObject().getPosition();
        auto orientation = m_player.getFocusedObject().getOrientation();

        Ogre::Quaternion ogreOrientation = m_graphics.toOgreQuaternion(orientation);

        int top = y - (m_graphics.getHeight() / 2);
        int left = x - (m_graphics.getWidth() / 2);

        top *= 100;

        Ogre::Vector3 delta(left, -top, 0); 
        delta = ogreOrientation * delta;

        m_playerActionService.setFocusedObjectCourse(position + m_graphics.toPosition(delta));
    }
}

void PilotView::actionClicked(MyGUI::Widget * widget)
{
    LOG_DEBUG << "Action clicked";

    auto * action = widget->getUserData<ActionType>();
    auto actionId = action->get<0>();
    auto actionParameter = action->get<1>();

    m_playerActionService.executeAction(actionId);

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

void PilotView::availableActionsFetched(std::vector<boost::tuple<int, int, std::string> > actions)
{
    auto * actionsPanel = m_gui->findWidget<MyGUI::Widget>("ActionsPanel");

    LOG_DEBUG << "Got actions";

    // delete action buttons
    for (auto & actionButton: m_actionButtons)
    {
        m_gui->destroyWidget(actionButton);
    }
    m_actionButtons.clear();

    int buttonTop = 0;
    for (auto & action: actions)
    {
        LOG_DEBUG << "  " << action.get<0>() << "/" << action.get<1>();

        auto * actionButton = actionsPanel->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(0, buttonTop, 50, 50), MyGUI::Align::Default);
        m_actionButtons.push_back(actionButton);
        buttonTop += 50;

        actionButton->setCaption(action.get<2>());
        actionButton->setUserData(action);
        actionButton->eventMouseButtonClick += MyGUI::newDelegate(this, &PilotView::actionClicked);
    }
}

void PilotView::shipListBoxSelected(MyGUI::ListBox * listBox, size_t index)
{
    unsigned * id = listBox->getItemDataAt<unsigned>(index);

    LOG_DEBUG << "Changing focus to: " << *id;

    auto & ship = m_universe.getById<Common::Game::Object::Ship>(*id);

    m_playerActionService.focusObject(ship);
    m_playerActionService.fetchAvailableActions();
}

void PilotView::playerShipsFetched()
{
    LOG_DEBUG << "Got player ships";

    updatePlayerShipsListBox();
}

void PilotView::objectAdded(Common::Game::Object::ObjectBase & object)
{
    auto * ship = dynamic_cast<Common::Game::Object::Ship*>(&object);
    if (ship && ship->getOwnerId() == m_player.getId())
    {
        LOG_DEBUG << "Player ship added";
        updatePlayerShipsListBox();
    }
}

void PilotView::updatePlayerShipsListBox()
{
    MyGUI::ListBox * shipListBox = m_gui->findWidget<MyGUI::ListBox>("ShipListBox");

    shipListBox->removeAllItems();

    auto ships = m_universe.getByOwnerId<Common::Game::Object::Ship>(m_player.getId());
    for (auto ship: ships)
    {
        std::stringstream ss;
        ss << "Ship " << ship->getId();
        shipListBox->addItem(ss.str(), MyGUI::Any(ship->getId()));
    }
}

