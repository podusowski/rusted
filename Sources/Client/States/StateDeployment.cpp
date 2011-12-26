#include "Common/Logger/Logger.hpp"
#include "Client/States/StateDeployment.hpp"

using namespace Client::States;

StateDeployment::StateDeployment(Graphics::IGraphics & graphics,
                                 Input::Input & input,
                                 Gui::Gui & gui, 
                                 Network::Connection & connection, 
                                 Common::Thread & thread) :
    m_connection(connection),
    m_connectionThread(thread),

    m_authorizationService(connection, m_playerInfo),
    m_rustedTimeService(connection, m_time),
    m_entityService(connection, m_time, m_playerInfo, m_universe),
    m_staticObjectService(connection),

    m_staticObjectView(m_staticObjectService, graphics),
    m_pilotView(graphics, input, m_entityService),

    m_pilotState(m_stateManager, graphics, gui, m_entityService, m_staticObjectView, m_pilotView),
    m_entitySelect(m_stateManager, gui, connection, m_entityService, m_pilotState),
    m_loginState(m_stateManager, gui, m_entitySelect, m_authorizationService, m_rustedTimeService, *this)
{
    m_stateManager.pushState(m_loginState);
}

void StateDeployment::frameStarted()
{
    m_stateManager.frameStarted();
}

void StateDeployment::deployNewConnection()
{
    LOG_INFO << "Deploying new connection\n";

    m_connection.connect();
    m_connectionThread.start();
    m_connection.addListener(m_authorizationService);
}

void StateDeployment::deployAuthorizedConnection()
{
    LOG_INFO << "Deploying authorized connection\n";

    m_connection.addListener(m_rustedTimeService);
    m_connection.addListener(m_entityService);
    m_connection.addListener(m_staticObjectService);
}

