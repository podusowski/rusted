#include <stdexcept>
#include <signal.h>

#include "Cake/Diagnostics/Logger.hpp"
#include "Cake/Networking/ServerSocket.hpp"

#include "Server/Network/ServerController.hpp"

using namespace Server::Network;

ServerController::ServerController(int argc, const char * argv[]) :
    m_lastConnectionId(1),
    m_cfg(argc, argv),
    m_dbFactory(m_cfg),
    m_db(m_dbFactory.create()),
    m_playerContainer(m_db),
    m_serviceDeployment(m_cfg, m_db, m_playerContainer)
{
	struct sigaction sigact;
	memset(&sigact, 0, sizeof(sigact));

	sigact.sa_handler = handleSignal;
	::sigaction(15, &sigact, 0);
}

int ServerController::start()
{
    int tcpPort = m_cfg.getValue<int>("network.port");

    LOG_INFO << "I will listen on TCP/" << tcpPort;

    boost::shared_ptr<Cake::Networking::ServerSocket> server =
        Cake::Networking::ServerSocket::createTcpServer(tcpPort);

    try
    {
        while (true)
        {
            gc();
            boost::shared_ptr<Cake::Networking::Socket> socket = server->accept();

            LOG_DEBUG << "New connection established";

            boost::shared_ptr<ConnectionContext> connectionContext(new ConnectionContext(m_lastConnectionId++, socket, m_serviceDeployment));
            m_connections.push_back(connectionContext);
            m_playerContainer.add(connectionContext->getConnection());
            connectionContext->getThread().start();
        }
    }
    catch (std::exception & ex)
    {
        LOG_WARN << "Server is shutting down, reason: " << ex.what();
    }

    LOG_INFO << "Server is done";

	return 0;
}

void ServerController::gc()
{
    for (auto it = m_connections.begin(); it != m_connections.end(); it++)
    {
        if (not (*it)->getThread().isRunning())
        {
            LOG_DEBUG << "Collecting innactive connection: " << (*it)->getId();

            m_playerContainer.remove((*it)->getConnection());
            it = m_connections.erase(it);
        }
    }
}

void ServerController::handleSignal(int signum)
{
	if (signum == 15)
	{
		LOG_WARN << "SIGINT catched";
	}
}

