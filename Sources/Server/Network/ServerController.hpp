#pragma once

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

#include "Cake/Networking/Socket.hpp"

#include "Network/Connection.hpp"
#include "Server/Services/ServiceDeployment.hpp"
#include "Network/ConnectionContext.hpp"

namespace Server
{
namespace Network
{

class ServerController
{
public:
    ServerController(int argc, const char * argv[]);
    int start();
	
private:
    void gc();
    static void handleSignal(int signum);

    std::vector<boost::shared_ptr<ConnectionContext> > m_connections;
    int m_lastConnectionId;

    Common::Configuration::Configuration m_cfg;
    Server::Services::ServiceDeployment m_serviceDeployment;
    boost::function<void(ConnectionContext&)> m_connectionAddedCallback;
    boost::function<void(ConnectionContext&)> m_connectionRemovedCallback;
};

}
}
