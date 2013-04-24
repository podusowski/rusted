#pragma once

#include "Cake/Threading/Thread.hpp"

#include "Common/Game/Player.hpp"

#include "Client/Graphics/IGraphics.hpp"
#include "Client/Input/Input.hpp"
#include "Client/States/StateManager.hpp"
#include "Client/States/EntitySelectState.hpp"
#include "Client/States/LoginState.hpp"
#include "Client/States/PilotState.hpp"

#include "Client/Services/RustedTimeService.hpp"
#include "Client/Services/PlayerActionService.hpp"
#include "Client/Services/ObjectService.hpp"
#include "Client/Services/EffectsService.hpp"

#include "Client/States/IStateDeployment.hpp"
#include "Client/Views/ObjectsView.hpp"
#include "Client/Views/PilotView.hpp"
#include "Client/Views/ActionsView.hpp"

#include "Client/Visuals/VisualObjectContainer.hpp"

namespace Client
{
namespace States
{

class StateDeployment : public IStateDeployment
{
public:
    StateDeployment(Graphics::IGraphics &,
                    Effects::Effects &,
                    Input::Input &,
                    Gui::Gui & gui, 
                    Network::Connection &, 
                    Cake::Threading::Thread &);
    void frameStarted();

    void deployNewConnection();
    void deployAuthorizedConnection();

private:
    StateManager m_stateManager;
    Common::Game::Player m_playerInfo;
    Network::Connection & m_connection;
    Cake::Threading::Thread & m_connectionThread;
    Common::Game::Universe m_universe;

    Services::AuthorizationService m_authorizationService;
    Services::RustedTimeService m_rustedTimeService;
    Services::PlayerActionService m_playerActionService;
    Services::ObjectService m_objectService;
    Services::EffectsService m_effectsService;

    Visuals::VisualObjectContainer m_visualObjectContainer;

    Views::ObjectsView m_objectsView;
    Views::PilotView m_pilotView;
    Views::ActionsView m_actionsView;

    States::PilotState m_pilotState;
    States::EntitySelectState m_entitySelect;
    States::LoginState m_loginState;
};

}
}

