#pragma once

#include "Common/Game/Universe.hpp"
#include "Client/States/IState.hpp"
#include "Client/Gui/Gui.hpp"
#include "Client/Network/Connection.hpp"
#include "Client/States/IStateManagerStack.hpp"
#include "Client/Services/PlayerActionService.hpp"
#include "Client/States/PilotState.hpp"

namespace Client
{
namespace States
{

class EntitySelectState : public IState
{
public:
    EntitySelectState(IStateManagerStack &,
                      Gui::Gui &, 
                      Network::Connection &,
                      Services::PlayerActionService &,
                      Services::ObjectService &,
                      States::PilotState &,
                      Common::Game::Universe &,
                      Client::Game::PlayerInfo &);
    void activate();
    void deactivate();
    void frameStarted();

private:
    void myEntitiesFetched();

    States::IStateManagerStack & m_stateManagerStack;
    Gui::Gui & m_gui;
    Services::PlayerActionService & m_playerActionService;
    Services::ObjectService & m_objectService;
    States::PilotState & m_pilotState;
    Common::Game::Universe & m_universe;
    Client::Game::PlayerInfo & m_playerInfo;
};

}
}

