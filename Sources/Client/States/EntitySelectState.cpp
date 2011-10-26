#include <sstream>
#include <boost/bind.hpp>

#include "Common/Logger/Logger.hpp"
#include "Client/States/EntitySelectState.hpp"

using namespace Client::States;

EntitySelectState::EntitySelectState(IStateManagerStack & stateManagerStack,
                                     Gui::Gui & gui, 
                                     Network::Connection & connection,
                                     Services::EntityService & entityService,
                                     States::PilotState & pilotState) :
    m_stateManagerStack(stateManagerStack),
    m_gui(gui),
    m_entityService(entityService),
    m_pilotState(pilotState)
{
}

void EntitySelectState::activate()
{
    m_layout = m_gui.getCeguiWindowManager().loadWindowLayout("EntitySelectScreen.layout");
    CEGUI::System::getSingleton().setGUISheet(m_layout);
    m_layout->activate();

    m_entityService.fetchMyEntitiesInfo(boost::bind(&EntitySelectState::myEntitiesFetched, this));
}

void EntitySelectState::deactivate()
{
}

void EntitySelectState::frameStarted()
{
}

void EntitySelectState::myEntitiesFetched()
{
    Client::Game::EntityContainer & container = m_entityService.getEntityContainer();
    CEGUI::Listbox * entitiesListbox = dynamic_cast<CEGUI::Listbox*>(m_layout->getChildRecursive("EntitiesListbox"));
    std::vector<Common::Game::Entity *> myEntities = container.getMyEntities();

    BOOST_FOREACH(Common::Game::Entity * entity, myEntities)
    {
        std::stringstream ss;
        ss << "My entity " << entity->getId() << "\n";
        entitiesListbox->addItem(new MyListItem(ss.str(), entity->getId()));
        
        LOG_INFO << ss.str();
    }

    CEGUI::Window * flyButton = m_layout->getChildRecursive("FlyButton");
    flyButton->subscribeEvent(CEGUI::PushButton::EventClicked,
        CEGUI::Event::Subscriber(&EntitySelectState::flyButtonClicked, this));
}

bool EntitySelectState::flyButtonClicked(const CEGUI::EventArgs &)
{
    CEGUI::Listbox * entitiesListbox = dynamic_cast<CEGUI::Listbox*>(m_layout->getChildRecursive("EntitiesListbox"));
    Common::Game::Entity & entity = m_entityService.getEntityContainer().getEntity(entitiesListbox->getFirstSelectedItem()->getID());

    LOG_INFO << "Let's fly (entityId: " << entity.getId() << ")\n";

    m_entityService.setCurrentEntity(entity);
    m_stateManagerStack.pushState(m_pilotState);
    return true;
}
