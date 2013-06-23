#include <sstream>
#include <boost/bind.hpp>

#include "Cake/Diagnostics/Logger.hpp"

#include "Common/Game/Object/Ship.hpp"

#include "Services/ObjectService.hpp"

#include "EntitySelectState.hpp"

using namespace Client::States;

EntitySelectState::EntitySelectState(IStateManagerStack & stateManagerStack,
                                     Gui::Gui & gui, 
                                     Network::Connection & connection,
                                     Services::PlayerActionService & playerActionService,
                                     Services::ObjectService & objectService,
                                     States::PilotState & pilotState,
                                     Common::Game::Universe & universe,
                                     Common::Game::Player & playerInfo) :
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
    auto ships = m_universe.getByOwnerId<Common::Game::Object::Ship>(m_playerInfo.getId());

    if (ships.empty())
    {
        throw std::runtime_error("player has no ships to fly with");
    }

    Common::Game::Object::Ship & ship = dynamic_cast<Common::Game::Object::Ship&>(**ships.begin());

    m_playerActionService.focusObject(ship);
    m_stateManagerStack.pushState(m_pilotState);
}

