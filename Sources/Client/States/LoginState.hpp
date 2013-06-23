#pragma once

#include "Forwards.hpp"

#include "Client/States/IState.hpp"
#include "Client/States/IStateManagerStack.hpp"
#include "Client/Graphics/IGraphics.hpp"
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
               Graphics::IGraphics &,
               Gui::Gui &,
               IState & entitySelectState,
               Services::AuthorizationService &,
               Services::RustedTimeService &,
               States::IStateDeployment &);
    void activate();
    void deactivate();
    void frameStarted();

private:
    void loginButtonClicked(MyGUI::WidgetPtr);
    void quitButtonClicked(MyGUI::WidgetPtr);

    void loggedIn(bool success);

    Cake::DependencyInjection::Inject<Cake::Configuration::Configuration> m_cfg;
    Graphics::IGraphics & m_graphics;
    Gui::Gui & m_gui;
    Services::AuthorizationService & m_authorizationService;
    Services::RustedTimeService & m_rustedTimeService;
    IStateManagerStack & m_stateManagerStack;
    IState & m_entitySelectState;
    States::IStateDeployment & m_stateDeployment;
};

}
}

