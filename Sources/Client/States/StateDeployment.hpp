#pragma once

#include "Cake/Threading/Thread.hpp"

#include "Client/Graphics/IGraphics.hpp"
#include "Client/Input/Input.hpp"
#include "Client/States/StateManager.hpp"
#include "Client/States/EntitySelectState.hpp"
#include "Client/States/LoginState.hpp"
#include "Client/States/PilotState.hpp"

#include "Client/Services/RustedTimeService.hpp"
#include "Client/Services/EntityService.hpp"
#include "Client/Services/StaticObjectService.hpp"
#include "Client/Services/ObjectService.hpp"

#include "Client/Game/PlayerInfo.hpp"
#include "Client/States/IStateDeployment.hpp"
#include "Client/Views/ObjectsView.hpp"
#include "Client/Views/PilotView.hpp"

namespace Client
{
namespace States
{

class StateDeployment : public IStateDeployment
{
public:
    StateDeployment(Graphics::IGraphics &,
                    Input::Input &,
                    Gui::Gui & gui, 
                    Network::Connection &, 
                    Cake::Threading::Thread &);
    void frameStarted();

    void deployNewConnection();
    void deployAuthorizedConnection();

private:
    StateManager m_stateManager;
    Game::PlayerInfo m_playerInfo;
    Network::Connection & m_connection;
    Cake::Threading::Thread & m_connectionThread;
    Common::Game::Universe m_universe;

    Services::AuthorizationService m_authorizationService;
    Services::RustedTimeService m_rustedTimeService;
    Services::EntityService m_entityService;
    Services::StaticObjectService m_staticObjectService;
    Services::ObjectService m_objectService;

    Views::ObjectsView m_objectsView;
    Views::PilotView m_pilotView;

    States::PilotState m_pilotState;
    States::EntitySelectState m_entitySelect;
    States::LoginState m_loginState;
};

}
}

