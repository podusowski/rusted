#include <stdexcept>
#include <boost/asio.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include <signal.h>

#include <Common/Configuration/Configuration.hpp>
#include <Common/Thread.hpp>
#include "Common/Logger/Logger.hpp"

#include "Server/Network/ServerController.hpp"

using namespace ::Server;

bool ServerController::s_terminating = false;

ServerController::ServerController(int argc, const char * argv[]) :
    m_lastConnectionId(1),
    m_cfg(argc, argv),
    m_serviceDeployment(m_cfg)
{
    int tcpPort = m_cfg.getValue<int>("network.port");

    m_acceptor = new boost::asio::ip::tcp::acceptor(io_service, tcp::endpoint(tcp::v4(), tcpPort));
    LOG_INFO << "I will listen on TCP/" << tcpPort << "\n";
}

int ServerController::start()
{
	struct sigaction sigact;
	memset(&sigact, 0, sizeof(sigact));

	sigact.sa_handler = handleSignal;
	::sigaction(15, &sigact, 0);

    try
    {
	while (!s_terminating)
	{
		// TODO: store threads and handle disconnects
		tcp::socket * socket = new tcp::socket(io_service);
		m_acceptor->accept(*socket);

		LOG_INFO << "New connection (remote endpoint: " << socket->remote_endpoint() << ")\n";

		Server::Network::Connection * connection = new Server::Network::Connection(m_lastConnectionId++, *socket, m_serviceDeployment);


		::Common::Thread * thread = new ::Common::Thread(*connection);
		thread->start();

		m_connections.push_back(connection);
	}
    }
    catch (std::exception & ex)
    {
        LOG_WARN << "Shutting down the server with exception of type " << TYPENAME(ex) << "\n";
    }

	LOG_INFO << "Dropping the connections\n";
	BOOST_FOREACH(Server::Network::Connection * c, m_connections)
	{
		delete c;
	}

	m_acceptor->close();

    LOG_INFO << "Server is done.\n";

	return 0;
}

void ServerController::handleSignal(int signum)
{
	if (signum == 15)
	{
		LOG_WARN << "SIGINT catched\n";
		s_terminating = true;
	}
}

