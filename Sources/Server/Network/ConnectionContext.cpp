#include "ConnectionContext.hpp"

using namespace Server::Network;

ConnectionContext::ConnectionContext(boost::shared_ptr<Cake::Networking::Socket> socket, 
                                     Services::IServiceDeployment & serviceDeployment) :
    m_socket(socket),
    m_connection(*socket, serviceDeployment),
    m_thread(m_connection)
{
}

Cake::Threading::Thread & ConnectionContext::getThread()
{
    return m_thread;
}

IConnection & ConnectionContext::getConnection()
{
    return m_connection;
}

