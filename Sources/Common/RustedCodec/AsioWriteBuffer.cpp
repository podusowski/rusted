#include <RustedCodec/AsioWriteBuffer.hpp>

using namespace ::Common::RustedCodec;

AsioWriteBuffer::AsioWriteBuffer(::boost::asio::ip::tcp::socket & p_socket) :
		m_socket(p_socket)
{
}

void AsioWriteBuffer::write(const void * source, size_t size)
{
	::boost::asio::write(m_socket, ::boost::asio::buffer(source, size));
}
