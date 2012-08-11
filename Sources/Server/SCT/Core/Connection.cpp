#include "Cake/Diagnostics/Logger.hpp"

#include <Common/RustedCodec/AsioWriteBuffer.hpp>
#include <Common/RustedCodec/AsioReadBuffer.hpp>

#include "Connection.hpp"

using namespace ::SCT;
using namespace ::boost::asio;
using namespace ::boost::asio::ip;
using namespace ::Common::Messages;

Connection::Connection(const std::string & addr, unsigned port) :
    m_socket(io_service)
{
	tcp::endpoint endpoint(address_v4::from_string(addr.c_str()), port);
	m_socket.connect(endpoint);
}

Connection::~Connection()
{
    LOG_INFO << "Closing connection";
}

void Connection::send(::Common::Messages::AbstractMessage & message)
{
    LOG_INFO << "Sending " << message;

    Common::RustedCodec::AsioWriteBuffer buffer(m_socket);

    try
    {
        message.serialize(buffer);
    }
    catch (...)
    {
        LOG_WARN << "Server droped the connection";
    }
}

