#ifndef ASIO_WRITE_BUFFER_HPP
#define ASIO_WRITE_BUFFER_HPP

#include <boost/asio.hpp>

#include <RustedCodec/IWriteBuffer.hpp>

namespace Common
{

namespace RustedCodec
{

class AsioWriteBuffer : public IWriteBuffer
{
public:
	AsioWriteBuffer(::boost::asio::ip::tcp::socket & p_socket);
	void write(const void * source, size_t size);
private:
	::boost::asio::ip::tcp::socket & m_socket;
};

}

}

#endif
