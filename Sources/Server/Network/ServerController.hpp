#pragma once

#include <vector>
#include <boost/shared_ptr.hpp>

#include "Cake/Networking/Socket.hpp"
#include "Cake/Threading/Thread.hpp"

#include "Network/Connection.hpp"
#include "Server/Services/ServiceDeployment.hpp"

namespace Server
{
namespace Network
{

class ConnectionDeployment
{
public:
    ConnectionDeployment(unsigned id, 
                         boost::shared_ptr<Cake::Networking::Socket> socket, 
                         Services::IServiceDeployment & serviceDeployment);

    unsigned getId() { return m_id; }
    Cake::Threading::Thread & getThread();

private:
    unsigned m_id;
    boost::shared_ptr<Cake::Networking::Socket> m_socket;
    Connection m_connection;
    Cake::Threading::Thread m_thread;
};

/**
 * @brief Starts the server, listens for connections and manage Connection instances.
 */
class ServerController
{
public:
    ServerController(int argc, const char * argv[]);
    int start();
	
private:
    void gc();
    static void handleSignal(int signum);

    std::vector<boost::shared_ptr<ConnectionDeployment> > m_connections;
    int m_lastConnectionId;

    Common::Configuration::Configuration m_cfg;

    Server::Services::ServiceDeployment m_serviceDeployment;
};

}
}
