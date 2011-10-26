#include <Common/RustedCodec/AsioWriteBuffer.hpp>
#include <Common/RustedCodec/AsioReadBuffer.hpp>
#include "Common/Logger/Logger.hpp"

#include <SCT/Connection.hpp>

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
    m_socket->close();
}

void Connection::send(::Common::Messages::AbstractMessage & message)
{
    LOG_SCT << "Sending " << message << "\n";

    ::Common::RustedCodec::AsioWriteBuffer buffer(*m_socket);

    try
    {
        message.serialize(buffer);
    }
    catch (...)
    {
        LOG_SCT << "Server droped the connection\n";
    }
}

std::auto_ptr< AbstractMessage > Connection::receive()
{
    LOG_SCT << "Waiting for message...\n";

    Common::RustedCodec::AsioReadBuffer buffer(*m_socket);
    std::auto_ptr< AbstractMessage > message;

    try
    {
        message = MessageFactory::create(buffer);
    }
    catch (...)
    {
        LOG_SCT << "Server droped the connection\n";
        throw;
    }

    LOG_SCT << "Received " << *message << "\n";

    return message;
}
