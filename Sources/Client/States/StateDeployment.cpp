#include "Cake/Diagnostics/Logger.hpp"

#include "Client/States/StateDeployment.hpp"

using namespace Client::States;

StateDeployment::StateDeployment(Graphics::IGraphics & graphics,
                                 Effects::Effects & effects,
                                 Input::Input & input,
                                 Gui::Gui & gui, 
                                 Network::Connection & connection, 
                                 Cake::Threading::Thread & thread) :
    m_connection(connection),
    m_connectionThread(thread),

    m_authorizationService(connection, m_playerInfo),
    m_rustedTimeService(connection),
    m_playerActionService(connection, m_playerInfo, m_universe),
    m_objectService(connection, m_universe),
    m_effectsService(effects),

    m_visualUniverse(graphics, gui, input, m_objectService),

    m_objectsView(m_objectService, m_playerActionService, graphics, effects, input, gui, m_universe, m_visualUniverse),
    m_pilotView(graphics, input, m_playerActionService, m_objectService, gui, m_universe, m_playerInfo, m_visualUniverse),
    m_actionsView(m_playerActionService, gui),

    m_pilotState(m_stateManager, graphics, gui, m_playerActionService, m_objectsView, m_pilotView, m_actionsView),
    m_entitySelect(m_stateManager, gui, connection, m_playerActionService, m_objectService, m_pilotState, m_universe, m_playerInfo),
    m_loginState(m_stateManager, graphics, gui, m_entitySelect, m_authorizationService, m_rustedTimeService, *this)
{
    m_stateManager.pushState(m_loginState);
}

void StateDeployment::frameStarted()
{
    m_stateManager.frameStarted();
}

void StateDeployment::deployNewConnection()
{
    LOG_DEBUG << "Deploying new connection";

    m_connection.connect();
    m_connectionThread.start();
    m_connection.addListener(m_authorizationService);
}

void StateDeployment::deployAuthorizedConnection()
{
    LOG_DEBUG << "Deploying authorized connection";

    m_connection.addListener(m_rustedTimeService);
    m_connection.addListener(m_playerActionService);
    m_connection.addListener(m_objectService);
    m_connection.addListener(m_effectsService);
}

