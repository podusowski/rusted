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
    m_camera(graphics, input, player)
{
}

PilotView::~PilotView()
{
}

void PilotView::activate()
{
    m_input.addMouseListener(*this);
    m_universe.addObjectAddedCallback(boost::bind(&PilotView::objectAdded, this, _1));

    m_objectService.fetchPlayerShips(boost::bind(&PilotView::playerShipsFetched, this));

    MyGUI::ListBox * shipListBox = m_gui->findWidget<MyGUI::ListBox>("ShipListBox");
    shipListBox->eventListChangePosition += MyGUI::newDelegate(this, &PilotView::shipListBoxSelected);

    createOrientationPlane();
    createCourseMarker();
}

void PilotView::deactivate()
{
}

void PilotView::frameStarted()
{
    updateShipPosition();
    updateFocusedShipWindow();
    m_camera.update();
    updateOrientationPlane();
    updateCourseMarker();
}

void PilotView::updateShipPosition()
{
    auto & focusedShip = dynamic_cast<Common::Game::Object::Ship&>(m_player.getFocusedObject());

    std::stringstream ss;
    ss << "Position: " << focusedShip.getPosition();

    auto description = focusedShip.getTrajectoryDescription();

    /*
    if (focusedShip.getPosition() != description.destination)
    {
        ss << "\nDestination: " << description.destination;
    }
    */

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

        //Ogre::Quaternion ogreOrientation = m_graphics.toOgreQuaternion(orientation);
        auto ogreOrientation = m_orientationPlaneSceneNode->getOrientation();

        int top = y - (m_graphics.getHeight() / 2);
        int left = x - (m_graphics.getWidth() / 2);

        top *= 100;
        left *= 10;

        Ogre::Vector3 delta(left, 0, top); 
        delta = ogreOrientation * delta;

        m_playerActionService.setFocusedObjectCourse(position + m_graphics.toPosition(delta));

        m_camera.resetUserOrientation();
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

void PilotView::updateFocusedShipWindow()
{
    auto & focusedShip = dynamic_cast<Common::Game::Object::Ship&>(m_player.getFocusedObject());

    std::stringstream ss;

    focusedShip.visitCargoHold([&](Common::Game::Object::CargoHold & cargoHold) -> void
    {
        ss << "C: " << cargoHold.getCarbon() << " H: " << cargoHold.getHelium() << " / " << cargoHold.getCapacity();
    });

    m_gui->findWidget<MyGUI::TextBox>("FocusTextBox")->setCaption(ss.str());
}

void PilotView::createOrientationPlane()
{
    Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
    Ogre::MeshManager::getSingleton().createPlane(
        "OrientationPlane.vmesh", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane, 2400, 1800, 20, 20, true, 1, 1, 1, Ogre::Vector3::UNIT_Z);

    Ogre::Entity * entGround = m_graphics.getSceneManager().createEntity("GroundEntity", "OrientationPlane.vmesh");
    entGround->setMaterialName("Grid");
    entGround->setCastShadows(false);

    m_orientationPlaneSceneNode = m_graphics.getSceneManager().getRootSceneNode()->createChildSceneNode();
    m_orientationPlaneSceneNode->attachObject(entGround);
}

void PilotView::updateOrientationPlane()
{
    m_orientationPlaneSceneNode->setVisible(m_camera.isUserOrientationChanging());

    auto & focusedShip = dynamic_cast<Common::Game::Object::Ship&>(m_player.getFocusedObject());
    auto position = focusedShip.getPosition();
    auto orientation = focusedShip.getOrientation();
    m_orientationPlaneSceneNode->setPosition(position.getX(), position.getY(), position.getZ());
    m_orientationPlaneSceneNode->setOrientation(m_camera.getOrientation());

    // orintation
    m_orientationPlaneSceneNode->pitch(Ogre::Degree(20));
}

void PilotView::createCourseMarker()
{
    Ogre::SceneManager & scene = m_graphics.getSceneManager();
    std::string mesh = "CourseMarker.mesh";
    m_courseMarkerEntity = scene.createEntity(mesh);
    m_courseMarkerNode = scene.getRootSceneNode()->createChildSceneNode();
    m_courseMarkerNode->setScale(100.0, 100.0, 100.0);
    m_courseMarkerNode->attachObject(m_courseMarkerEntity);
}

void PilotView::updateCourseMarker()
{
    auto & focusedShip = dynamic_cast<Common::Game::Object::Ship&>(m_player.getFocusedObject());
    m_courseMarkerNode->setPosition(m_graphics.toOgreVector3(focusedShip.getCourseMarkerPosition()));
}

