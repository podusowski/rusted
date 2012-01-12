#include <stdexcept>
#include <memory>
#include <boost/foreach.hpp>

#include "Cake/Diagnostics/Logger.hpp"

#include "Common/RustedCodec/CakeReadBuffer.hpp"
#include "Common/RustedCodec/CakeWriteBuffer.hpp"
#include "Common/Messages/Messages.hpp"

#include "Connection.hpp"

using namespace Server::Network;

Connection::Connection(int id, Cake::Networking::Socket & socket, Services::IServiceDeployment & serviceDeployment) :
        m_id(id),
        m_socket(socket)
{
    serviceDeployment.deployNewConnection(*this);
}

void Connection::addListener(IConnectionListener & listener)
{
    LOG_DEBUG << "Adding " << TYPENAME(listener) << " as message handler for connection: " << m_id;

    // TODO: this method can be called from different thread
    m_listenersToAdd.push_back(&listener);
}

void Connection::send(::Common::Messages::AbstractMessage & message)
{
    LOG_DEBUG << "CON#" << m_id << " Sending: " << message;
    Common::RustedCodec::CakeWriteBuffer buffer(m_socket);
    message.serialize(buffer);
}

void Connection::run()
{
    using namespace ::Common::Messages;

    Common::RustedCodec::IReadBuffer * buffer = new Common::RustedCodec::CakeReadBuffer(m_socket);

    while (true)
    {
        try
        {
            // FIXME: this is not thread safe!!
            // handle listeners operations
            BOOST_FOREACH( IConnectionListener * listener, m_listenersToAdd )
            {
                m_listeners.push_back(listener);
            }
            m_listenersToAdd.clear();

            // receive message
            std::auto_ptr< AbstractMessage > message = MessageFactory::create(*buffer);

            LOG_DEBUG << "CON#" << m_id << " Received: " << *message;

            for (std::vector< IConnectionListener * >::iterator it = m_listeners.begin(); it != m_listeners.end(); it++)
            {
                (*it)->messageReceived(*this, *message);
            }
        }
        catch (std::exception & ex)
        {
            LOG_DEBUG << "Connection dropped (id: " << m_id << ", reason: " << ex.what() << ")";
            break;
        }
        catch (...)
        {
            LOG_ERR << "Unknown exception was thrown";
        }
    }
}

int Connection::getId()
{
    return m_id;
}

