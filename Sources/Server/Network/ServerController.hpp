#pragma once

#include <vector>

#include <Network/Connection.hpp>
#include "Server/Services/ServiceDeployment.hpp"

namespace Server
{

/**
 * @brief Starts the server, listens for connections and manage Connection instances.
 */
class ServerController
{
public:
    ServerController(int argc, const char * argv[]);
    int start();
	
private:
    static void handleSignal(int signum);

    std::vector<Server::Network::Connection *> m_connections;
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::acceptor * m_acceptor;
    static bool s_terminating;
    int m_lastConnectionId;

    Common::Configuration::Configuration m_cfg;

    Server::Services::ServiceDeployment m_serviceDeployment;
};

}
