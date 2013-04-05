#include <stdexcept>
#include <memory>
#include <boost/foreach.hpp>

#include "Cake/Diagnostics/Logger.hpp"
#include "Cake/Threading/ScopedLock.hpp"

#include "Common/RustedCodec/CakeReadBuffer.hpp"
#include "Common/RustedCodec/CakeWriteBuffer.hpp"
#include "Common/Messages/Messages.hpp"

#include "Connection.hpp"

using namespace Server::Network;

Connection::Connection(Cake::Networking::Socket & socket, Services::IServiceDeployment & serviceDeployment) :
        m_socket(socket)
{
    serviceDeployment.deployNewConnection(*this);
}

Connection::~Connection()
{
    LOG_DEBUG << "Connection: " << this << " destroyed";
}

void Connection::addListener(IConnectionListener & listener)
{
    LOG_DEBUG << "Adding " << TYPENAME(listener) << " as message handler for connection: " << this;

    // this method can be called in loop where listeners are notified about message,
    // and we don't want to modify collection during its iteration
    m_listenersToAdd.push_back(&listener);
}

void Connection::send(const Common::Messages::AbstractMessage & message)
{
    LOG_DEBUG << "<connection:" << this << "> Sending: " << message;

    Cake::Threading::ScopedLock lock(m_socketMutex);

    Common::RustedCodec::CakeWriteBuffer buffer(m_socket);
    message.serialize(buffer);
}

void Connection::run()
{
    using namespace ::Common::Messages;

    Common::RustedCodec::CakeReadBuffer buffer(m_socket);

    try
    {
        while (true)
        {
            // handle listeners operations
            BOOST_FOREACH( IConnectionListener * listener, m_listenersToAdd )
            {
                m_listeners.push_back(listener);
            }
            m_listenersToAdd.clear();

            // receive message
            boost::shared_ptr<AbstractMessage> message = MessageFactory::create(buffer);

            LOG_DEBUG << "<connection:" << this << "> Received: " << *message;

            for (std::vector< IConnectionListener * >::iterator it = m_listeners.begin(); it != m_listeners.end(); it++)
            {
                (*it)->messageReceived(*this, *message);
            }
        }
    }
    catch (std::exception & ex)
    {
        LOG_DEBUG << "Connection dropped (id: " << this << ", reason: " << ex.what() << ")";
    }
    catch (...)
    {
        LOG_ERR << "Unknown exception was thrown";
    }
}

