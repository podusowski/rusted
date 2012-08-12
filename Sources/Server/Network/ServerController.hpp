#pragma once

#include <vector>
#include <boost/shared_ptr.hpp>

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
    ServerController();
    int start();
	
private:
    void gc();
    static void handleSignal(int signum);

    std::vector<boost::shared_ptr<ConnectionContext> > m_connections;
    int m_lastConnectionId;

    Cake::DependencyInjection::Inject<Cake::Configuration::Configuration> m_cfg;
    Cake::DependencyInjection::Inject<Server::DataBase::DataBase> m_db;
    Server::Game::PlayerContainer m_playerContainer;
    Server::Services::ServiceDeployment m_serviceDeployment;
};

}
}
