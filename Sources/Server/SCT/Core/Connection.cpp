#include "Cake/Diagnostics/Logger.hpp"

#include <Common/RustedCodec/AsioWriteBuffer.hpp>
#include <Common/RustedCodec/AsioReadBuffer.hpp>

#include "Connection.hpp"

using namespace ::SCT;
using namespace ::boost::asio;
using namespace ::boost::asio::ip;
using namespace ::Common::Messages;

Connection::Connection(const std::string & addr, unsigned port)
{
	m_socket = new tcp::socket(io_service);

	tcp::endpoint endpoint(address_v4::from_string(addr.c_str()), port);
	m_socket->connect(endpoint);
}

Connection::~Connection()
{
    LOG_INFO << "Closing connection";

    m_socket->close();
}

void Connection::send(::Common::Messages::AbstractMessage & message)
{
    LOG_INFO << "Sending " << message;

    ::Common::RustedCodec::AsioWriteBuffer buffer(*m_socket);

    try
    {
        message.serialize(buffer);
    }
    catch (...)
    {
        LOG_WARN << "Server droped the connection";
    }
}

std::auto_ptr< AbstractMessage > Connection::receive()
{
    LOG_INFO << "Waiting for message...";

    Common::RustedCodec::AsioReadBuffer buffer(*m_socket);
    std::auto_ptr< AbstractMessage > message;

    try
    {
        message = MessageFactory::create(buffer);
    }
    catch (...)
    {
        LOG_INFO << "Server droped the connection";
        throw;
    }

    LOG_INFO << "Received " << *message;

    return message;
}
