#ifndef COMMON_RUSTEDCODEC_SIMPLE_READ_BUFFER
#define COMMON_RUSTEDCODEC_SIMPLE_READ_BUFFER

#include <vector>

#include <RustedCodec/IReadBuffer.hpp>

namespace Common
{

namespace RustedCodec
{

/**
 * @brief Simple buffer implementation using vector of char.
 */
class SimpleReadBuffer : public IReadBuffer
{
public:
	SimpleReadBuffer(const std::vector<char> & data);
	void read(void * destinationBuffer, size_t size);

private:
	std::vector<char>::const_iterator m_rawBufferIterator;
};

}

}

#endif
