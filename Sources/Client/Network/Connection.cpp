#include "Cake/Diagnostics/Logger.hpp"

#include "Cake/Networking/Protocol/CakeReadBuffer.hpp"
#include "Cake/Networking/Protocol/CakeWriteBuffer.hpp"

#include "Network/Connection.hpp"

using namespace Client::Network;

Connection::Connection(Cake::Configuration::Configuration & cfg) :
    m_cfg(cfg)
{
    // better keep getValue on this in constructor, they're serving as asserts
    LOG_INFO << "Server address is " << m_cfg.getValue<std::string>("network.address")
             << ":" << m_cfg.getValue<unsigned>("network.port");
}

void Connection::run()
{
    LOG_DEBUG << "Connection service is in loop";
    while (true)
    {
        using namespace Common::Messages;

        Cake::Networking::Protocol::CakeReadBuffer buffer(*m_socket);
        std::shared_ptr<AbstractMessage> message = MessageFactory::create(buffer);

        LOG_DEBUG << "Message received: " << *message << ", putting it on the queue";
        m_messages.push(message);
        //FIXME: make a lock here
    }
}

void Connection::connect()
{
    std::string address = m_cfg.getValue<std::string>("network.address");
    unsigned port = m_cfg.getValue<unsigned>("network.port");

    LOG_INFO << "Connecting to host";
    m_socket = Cake::Networking::Socket::connectToTcpSocket(address, port);

    LOG_INFO << "Connected to " << address << ":" << port;
}

void Connection::addListener(IConnectionListener & listener)
{
    LOG_DEBUG << "Adding listener (name: " << TYPENAME(listener) << ")";
    m_listeners.push_back(&listener);
}

void Connection::send(Common::Messages::AbstractMessage & message)
{
    LOG_DEBUG << "Sending message: " << message;
    Cake::Networking::Protocol::CakeWriteBuffer buffer(*m_socket);
    message.serialize(buffer);
}

void Connection::yield()
{
    if (!m_messages.empty())
    {
        LOG_DEBUG << "Yielded while message queue is not empty (messages waiting: " << m_messages.size() << ")";

        auto message = m_messages.front();
        m_messages.pop();

        for (IConnectionListener * listener : m_listeners)
        {
            listener->messageReceived(*message);
        }
    }
}

