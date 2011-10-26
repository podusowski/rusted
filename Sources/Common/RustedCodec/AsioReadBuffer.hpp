#ifndef ASIO_READ_BUFFER_HPP
#define ASIO_READ_BUFFER_HPP

#include <boost/asio.hpp>

#include <RustedCodec/IReadBuffer.hpp>

namespace Common
{

namespace RustedCodec
{

class AsioReadBuffer : public IReadBuffer
{
public:
	AsioReadBuffer(::boost::asio::ip::tcp::socket & p_socket);
	void read(void * destinationBuffer, size_t size);
private:
	::boost::asio::ip::tcp::socket & m_socket;
};

}

}

#endif
