#include <stdexcept>
#include <boost/lexical_cast.hpp>

#include <signal.h>

#include "Cake/Diagnostics/Logger.hpp"
#include "Cake/Networking/ServerSocket.hpp"

#include "Server/Network/ServerController.hpp"

using namespace Server::Network;

ConnectionDeployment::ConnectionDeployment(unsigned id, 
                                           boost::shared_ptr<Cake::Networking::Socket> socket, 
                                           Services::IServiceDeployment & serviceDeployment) :
    m_id(id),
    m_socket(socket),
    m_connection(id, *socket, serviceDeployment),
    m_thread(m_connection)
{
}

Cake::Threading::Thread & ConnectionDeployment::getThread()
{
    return m_thread;
}

ServerController::ServerController(int argc, const char * argv[]) :
    m_lastConnectionId(1),
    m_cfg(argc, argv),
    m_serviceDeployment(m_cfg)
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

    while (true)
    {
        gc();
        boost::shared_ptr<Cake::Networking::Socket> socket = server->accept();

        LOG_DEBUG << "New connection established";

        boost::shared_ptr<ConnectionDeployment> connection(new ConnectionDeployment(m_lastConnectionId++, socket, m_serviceDeployment));
        m_connections.push_back(connection);
        connection->getThread().start();
    }

    LOG_INFO << "Server is done";

	return 0;
}

void ServerController::gc()
{
    for (std::vector<boost::shared_ptr<ConnectionDeployment> >::iterator it = m_connections.begin(); 
         it != m_connections.end(); it++)
    {
        if (not (*it)->getThread().isRunning())
        {
            LOG_DEBUG << "Collecting innactive connection: " << (*it)->getId();

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

