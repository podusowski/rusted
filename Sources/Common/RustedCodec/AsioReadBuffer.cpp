#include <RustedCodec/AsioReadBuffer.hpp>

using namespace ::Common::RustedCodec;

AsioReadBuffer::AsioReadBuffer(::boost::asio::ip::tcp::socket & p_socket) :
		m_socket(p_socket)
{
}

void AsioReadBuffer::read(void * destinationBuffer, size_t size)
{
    ::boost::asio::read(m_socket,
    			::boost::asio::buffer(destinationBuffer, size));

    // TODO: if reply_length < size
}
