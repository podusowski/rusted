#include <cstring>

#include "RustedCodec/RustedAbstractCoder.hpp"

using ::Common::RustedCodec::RustedAbstractCoder;

RustedAbstractCoder::RustedAbstractCoder(IWriteBuffer & p_buffer) :
		m_buffer(p_buffer)
{
}

RustedAbstractCoder & RustedAbstractCoder::operator << (const int p_int)
{
	m_buffer.write(&p_int, sizeof(p_int));
	return *this;
}

RustedAbstractCoder & RustedAbstractCoder::operator << (const bool p_value)
{
	m_buffer.write(&p_value, sizeof(p_value));
	return *this;
}

RustedAbstractCoder & RustedAbstractCoder::operator << (const std::string & p_str)
{
	*this << p_str.length();

	m_buffer.write(p_str.c_str(), p_str.length());
	return *this;
}

RustedAbstractCoder & RustedAbstractCoder::operator << (const size_t p_value)
{
	m_buffer.write(&p_value, sizeof(p_value));
	return *this;
}
