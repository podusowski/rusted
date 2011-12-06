#include <cstring>

#include <Common/RustedCodec/SimpleReadBuffer.hpp>

using namespace ::Common::RustedCodec;

SimpleReadBuffer::SimpleReadBuffer(const std::vector<char> & data) : m_rawBufferIterator(data.begin())
{
}

void SimpleReadBuffer::read(void * destinationBuffer, size_t size)
{
	::memcpy(destinationBuffer, &(*m_rawBufferIterator), size);
	m_rawBufferIterator += size;
}
