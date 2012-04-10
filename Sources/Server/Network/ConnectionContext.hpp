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
    ConnectionContext(unsigned id, 
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

}
}
