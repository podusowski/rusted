#include "ConnectionContext.hpp"

using namespace Server::Network;

ConnectionContext::ConnectionContext(unsigned id, 
                                     boost::shared_ptr<Cake::Networking::Socket> socket, 
                                     Services::IServiceDeployment & serviceDeployment) :
    m_id(id),
    m_socket(socket),
    m_connection(id, *socket, serviceDeployment),
    m_thread(m_connection)
{
}

Cake::Threading::Thread & ConnectionContext::getThread()
{
    return m_thread;
}

