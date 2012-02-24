#include <sstream>
#include <boost/bind.hpp>

#include "Cake/Diagnostics/Logger.hpp"

#include "Common/Game/Object/Ship.hpp"
#include "Client/States/EntitySelectState.hpp"

using namespace Client::States;

EntitySelectState::EntitySelectState(IStateManagerStack & stateManagerStack,
                                     Gui::Gui & gui, 
                                     Network::Connection & connection,
                                     Services::EntityService & entityService,
                                     Services::ObjectService & objectService,
                                     States::PilotState & pilotState,
                                     Common::Game::Universe & universe,
                                     Client::Game::PlayerInfo & playerInfo) :
    m_stateManagerStack(stateManagerStack),
    m_gui(gui),
    m_entityService(entityService),
    m_objectService(objectService),
    m_pilotState(pilotState),
    m_universe(universe),
    m_playerInfo(playerInfo)
{
}

void EntitySelectState::activate()
{
    m_layout = m_gui.getCeguiWindowManager().loadWindowLayout("EntitySelectScreen.layout");
    CEGUI::System::getSingleton().setGUISheet(m_layout);
    m_layout->activate();

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
    CEGUI::Listbox * entitiesListbox = dynamic_cast<CEGUI::Listbox*>(m_layout->getChildRecursive("EntitiesListbox"));

    std::vector<boost::shared_ptr<Common::Game::Object::ObjectBase> > myEntities
        = m_universe.getByOwnerId<Common::Game::Object::Ship>(m_playerInfo.getId());

    BOOST_FOREACH(boost::shared_ptr<Common::Game::Object::ObjectBase> ship, myEntities)
    {
        std::stringstream ss;
        ss << "My ship " << ship->getId() << "\n";
        entitiesListbox->addItem(new MyListItem(ss.str(), ship->getId()));
        
        LOG_INFO << ss.str();
    }

    CEGUI::Window * flyButton = m_layout->getChildRecursive("FlyButton");
    flyButton->subscribeEvent(CEGUI::PushButton::EventClicked,
        CEGUI::Event::Subscriber(&EntitySelectState::flyButtonClicked, this));
}

bool EntitySelectState::flyButtonClicked(const CEGUI::EventArgs &)
{
    CEGUI::Listbox * entitiesListbox = dynamic_cast<CEGUI::Listbox*>(m_layout->getChildRecursive("EntitiesListbox"));
    Common::Game::Object::Ship & ship = m_universe.getById<Common::Game::Object::Ship>(entitiesListbox->getFirstSelectedItem()->getID());

    LOG_INFO << "Ship selected (id: " << ship.getId() << ")";

    m_entityService.setCurrentEntity(ship);
    m_stateManagerStack.pushState(m_pilotState);
    return true;
}
