#pragma once

#include <boost/shared_ptr.hpp>

#include "Cake/Threading/Thread.hpp"
#include "Cake/Networking/Socket.hpp"

#include "Connection.hpp"
#include "Services/IServiceDeployment.hpp"

namespace Server
{
namespace Network
{

class ConnectionContext
{
public:
    ConnectionContext(std::shared_ptr<Cake::Networking::Socket> socket, 
                      Services::IServiceDeployment & serviceDeployment);

    Cake::Threading::Thread & getThread();
    Network::IConnection & getConnection();

private:
    std::shared_ptr<Cake::Networking::Socket> m_socket;
    Connection m_connection;
    Cake::Threading::Thread m_thread;
};

}
}

