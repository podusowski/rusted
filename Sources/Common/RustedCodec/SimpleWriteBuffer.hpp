#ifndef COMMON_RUSTEDCODEC_SIMPLEWRITE_BUFFER
#define COMMON_RUSTEDCODEC_SIMPLEWRITE_BUFFER

#include <vector>

#include <RustedCodec/IWriteBuffer.hpp>

namespace Common
{

namespace RustedCodec
{

/**
 * @brief Simple buffer implementation using vector of char.
 */
class SimpleWriteBuffer : public IWriteBuffer
{
public:
	SimpleWriteBuffer(std::vector<char> & rawBuffer);
	void write(const void * source, size_t size);
private:
	std::vector<char> & m_buffer;
};

}

}

#endif
