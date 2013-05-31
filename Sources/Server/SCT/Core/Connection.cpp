#include "Cake/Diagnostics/Logger.hpp"

#include "Common/RustedCodec/CakeWriteBuffer.hpp"
#include "Common/RustedCodec/CakeReadBuffer.hpp"

#include "Connection.hpp"

using namespace SCT;
using namespace Common::Messages;

Connection::Connection(const std::string & addr, unsigned port)
{
    m_socket = Cake::Networking::Socket::connectToTcpSocket(addr, port);
}

Connection::~Connection()
{
    LOG_INFO << "Closing connection";
}

void Connection::send(::Common::Messages::AbstractMessage & message)
{
    LOG_INFO << "Sending " << message;

    Common::RustedCodec::CakeWriteBuffer buffer(*m_socket);

    try
    {
        message.serialize(buffer);
    }
    catch (...)
    {
        LOG_WARN << "Server droped the connection";
    }
}

