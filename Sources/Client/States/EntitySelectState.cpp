#include <sstream>
#include <boost/bind.hpp>

#include "Cake/Diagnostics/Logger.hpp"

#include "Common/Game/Object/Ship.hpp"
#include "Client/States/EntitySelectState.hpp"

using namespace Client::States;

EntitySelectState::EntitySelectState(IStateManagerStack & stateManagerStack,
                                     Gui::Gui & gui, 
                                     Network::Connection & connection,
                                     Services::PlayerActionService & playerActionService,
                                     Services::ObjectService & objectService,
                                     States::PilotState & pilotState,
                                     Common::Game::Universe & universe,
                                     Client::Game::PlayerInfo & playerInfo) :
    m_stateManagerStack(stateManagerStack),
    m_gui(gui),
    m_playerActionService(playerActionService),
    m_objectService(objectService),
    m_pilotState(pilotState),
    m_universe(universe),
    m_playerInfo(playerInfo)
{
}

void EntitySelectState::activate()
{
    m_gui.loadLayout("EntitySelectScreen.layout");

    m_objectService.fetchPlayerShips(boost::bind(&EntitySelectState::myEntitiesFetched, this));
}

void EntitySelectState::deactivate()
{
}

void EntitySelectState::frameStarted()
{
}

void EntitySelectState::myEntitiesFetched()
{
    MyGUI::ListBox * shipListBox = m_gui->findWidget<MyGUI::ListBox>("ShipListBox");

    auto ships = m_universe.getByOwnerId<Common::Game::Object::Ship>(m_playerInfo.getId());
    for (auto ship: ships)
    {
        std::stringstream ss;
        ss << "My ship " << ship->getId();
        shipListBox->addItem(ss.str(), MyGUI::Any(ship->getId()));
    }

    m_gui->findWidget<MyGUI::Button>("FlyButton")->eventMouseButtonClick += MyGUI::newDelegate(this, &EntitySelectState::flyButtonClicked);
}

void EntitySelectState::flyButtonClicked(MyGUI::WidgetPtr)
{
    MyGUI::ListBox * shipListBox = m_gui->findWidget<MyGUI::ListBox>("ShipListBox");
    size_t idx = shipListBox->getIndexSelected();
    unsigned * shipId = shipListBox->getItemDataAt<unsigned>(idx);
    
    LOG_INFO << "Ship selected (id: " << *shipId << ")";

    Common::Game::Object::Ship & ship = m_universe.getById<Common::Game::Object::Ship>(*shipId);

    m_playerActionService.focusObject(ship);
    m_stateManagerStack.pushState(m_pilotState);
}
