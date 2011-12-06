#include <cstring>

#include <Common/RustedCodec/SimpleWriteBuffer.hpp>

using namespace ::Common::RustedCodec;

SimpleWriteBuffer::SimpleWriteBuffer(std::vector<char> & rawBuffer) :
		m_buffer(rawBuffer)
{
}

void SimpleWriteBuffer::write(const void * source, size_t size)
{
  	m_buffer.resize(m_buffer.size() + size);
	::memcpy(&(m_buffer[m_buffer.size() - size]), source, size);
}
