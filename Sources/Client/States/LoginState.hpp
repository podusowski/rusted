#pragma once

#include "Client/States/IState.hpp"
#include "Client/States/IStateManagerStack.hpp"
#include "Client/Gui/Gui.hpp"
#include "Client/Network/Connection.hpp"
#include "Client/Services/AuthorizationService.hpp"
#include "Client/Services/RustedTimeService.hpp"
#include "Client/States/IStateDeployment.hpp"

namespace Client
{
namespace States
{

class LoginState : public IState
{
public:
    LoginState(IStateManagerStack &,
               Gui::Gui &,
               IState & entitySelectState,
               Services::AuthorizationService &,
               Services::RustedTimeService &,
               States::IStateDeployment &);
    void activate();
    void deactivate();
    void frameStarted();

private:
    bool loginButtonClicked(const CEGUI::EventArgs &);
    void loggedIn(bool success);

    Gui::Gui & m_gui;
    Services::AuthorizationService & m_authorizationService;
    Services::RustedTimeService & m_rustedTimeService;
    IStateManagerStack & m_stateManagerStack;
    IState & m_entitySelectState;
    States::IStateDeployment & m_stateDeployment;
};

}
}

