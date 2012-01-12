#include <exception>

#include "Cake/Diagnostics/Logger.hpp"

#include "Common/RustedCodec/AsioReadBuffer.hpp"
#include "Common/RustedCodec/AsioWriteBuffer.hpp"
#include "Network/Connection.hpp"

using namespace boost::asio;
using namespace Client::Network;

Connection::Connection(Common::Configuration::Configuration & cfg) : 
    m_cfg(cfg),
    m_socket(io_service)
{
    // better keep getValue on this in constructor, they're serving as asserts
    LOG_INFO << "Server address is " << m_cfg.getValue<std::string>("network.address")
             << ":" << m_cfg.getValue<unsigned>("network.port");
}

void Connection::run()
{
    LOG_INFO << "Connection service is in loop";
    while (true)
    {
        using namespace Common::Messages;

        Common::RustedCodec::AsioReadBuffer buffer(m_socket);
        std::auto_ptr<AbstractMessage> message = MessageFactory::create(buffer);
        
        LOG_INFO << "Message received, " << TYPENAME(*message) << ", putting it on the queue";
        m_messages.push(message.release());
        //FIXME: make a lock here
    }
}

void Connection::connect()
{
    std::string address = m_cfg.getValue<std::string>("network.address");

    // following asio's naming convention...
    std::string serviceName = m_cfg.getValue<std::string>("network.port");

    tcp::resolver resolver(io_service);
    tcp::resolver::query query(address, serviceName);

    LOG_INFO << "Resolving " << address << ":" << serviceName;
    tcp::resolver::iterator endpointIterator = resolver.resolve(query);
    tcp::resolver::iterator end;

    boost::system::error_code error = boost::asio::error::host_not_found;

    while (error && endpointIterator != end)
    {
        LOG_INFO << "Connecting to host";
        m_socket.close();
        m_socket.connect(*endpointIterator, error);
        endpointIterator++;
    }

    if (error)
    {
        LOG_ERR << "Can't connect to the server";
        throw std::exception();
    }
    else
    {
        LOG_INFO << "Connected";
    }
}

void Connection::addListener(IConnectionListener & listener)
{
    LOG_INFO << "Adding listener (name: " << TYPENAME(listener) << ")";
    m_listeners.push_back(&listener);
}

void Connection::send(Common::Messages::AbstractMessage & message)
{
    LOG_INFO << "Sending " << TYPENAME(message);
    Common::RustedCodec::AsioWriteBuffer buffer(m_socket);
    message.serialize(buffer);
}

void Connection::yield()
{
    if (!m_messages.empty())
    {
        LOG_INFO << "Yielded while message queue is not empty (messages waiting: " << m_messages.size() << ")";

        Common::Messages::AbstractMessage * message = m_messages.front();
        m_messages.pop();

        BOOST_FOREACH(IConnectionListener * listener, m_listeners)
        {
            listener->messageReceived(*message);
        }
    }
}

