#include <stdexcept>
#include <signal.h>

#include "Cake/Diagnostics/Logger.hpp"
#include "Cake/Networking/ServerSocket.hpp"
#include "Cake/Networking/ServerSocketPool.hpp"

#include "Server/Network/ServerController.hpp"

using namespace Server::Network;

ServerController::ServerController() :
    m_lastConnectionId(1),
    m_playerContainer(),
    m_serviceDeployment(*m_cfg, m_playerContainer)
{
    struct sigaction sigact;
    memset(&sigact, 0, sizeof(sigact));

    sigact.sa_handler = handleSignal;
    ::sigaction(15, &sigact, 0);
}

int ServerController::start()
{
    int tcpPort = m_cfg->getValue<int>("network.port");
    std::string administrationSocketPath = m_cfg->getValue<std::string>("network.administration_socket_path");

    LOG_INFO << "Setting up player socket on TCP/" << tcpPort;
    auto server = Cake::Networking::ServerSocket::createTcpServer(tcpPort);

    LOG_INFO << "Setting up administrative socket on UNIX/" << administrationSocketPath;
    auto administrationServer = Cake::Networking::ServerSocket::createUnixServer(administrationSocketPath);

    Cake::Networking::ServerSocketPool serverPool;
    serverPool.add(server);
    serverPool.add(administrationServer);

    LOG_INFO << "Accepting connections";

    try
    {
        while (true)
        {
            gc();

            auto socket = serverPool.accept();

            if (socket.second == server)
            {
                LOG_DEBUG << "New connection established";

                boost::shared_ptr<ConnectionContext> connectionContext(new ConnectionContext(socket.first, m_serviceDeployment));

                m_connections.push_back(connectionContext);
                m_playerContainer.add(connectionContext->getConnection());
                m_serviceDeployment.deployNewConnection(connectionContext->getConnection());
                connectionContext->getThread().start();
            }
            else
            {
                LOG_ERR << "Administration connection not implemented";
            }
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
    auto it = m_connections.begin();
    while (it != m_connections.end())
    {
        if (not (*it)->getThread().isRunning())
        {
            LOG_DEBUG << "Collecting innactive connection: " << (*it);

            m_playerContainer.remove((*it)->getConnection());
            it = m_connections.erase(it);
        }
        else
        {
            ++it;
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

