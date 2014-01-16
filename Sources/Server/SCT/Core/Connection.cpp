#include "Cake/Diagnostics/Logger.hpp"

#include "Cake/Networking/Protocol/CakeWriteBuffer.hpp"
#include "Cake/Networking/Protocol/CakeReadBuffer.hpp"

#include "Connection.hpp"

using namespace SCT;
using namespace Common::Messages;

Connection::Connection(Connection::Type type, const std::string & addr, unsigned port)
{
    if (type == Connection::Type::TCP)
    {
        LOG_INFO << "Connecting to TCP:" << addr;
        m_socket = Cake::Networking::Socket::connectToTcpSocket(addr, port);
    }
    else
    {
        LOG_INFO << "Connecting to UNIX:" << addr;
        m_socket = Cake::Networking::Socket::connectToUnixSocket(addr);
    }
}

Connection::~Connection()
{
    LOG_INFO << "Closing connection";
}

void Connection::send(::Common::Messages::AbstractMessage & message)
{
    LOG_INFO << "Sending " << message;

    Cake::Networking::Protocol::CakeWriteBuffer buffer(*m_socket);

    try
    {
        message.serialize(buffer);
    }
    catch (...)
    {
        LOG_WARN << "Server droped the connection";
    }
}

