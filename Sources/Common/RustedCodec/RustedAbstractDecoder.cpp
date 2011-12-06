#include <cstring>

#include <RustedCodec/RustedAbstractDecoder.hpp>

using ::Common::RustedCodec::RustedAbstractDecoder;

RustedAbstractDecoder::RustedAbstractDecoder(IReadBuffer & p_buffer) :
		m_buffer(p_buffer)
{
}

RustedAbstractDecoder & RustedAbstractDecoder::operator >> (int & p_int)
{
	m_buffer.read(&p_int, sizeof(p_int));
	return *this;
}

RustedAbstractDecoder & RustedAbstractDecoder::operator >> (bool & p_value)
{
	m_buffer.read(&p_value, sizeof(p_value));
	return *this;
}

RustedAbstractDecoder & RustedAbstractDecoder::operator >> (size_t & p_value)
{
	m_buffer.read(&p_value, sizeof(p_value));
	return *this;
}

RustedAbstractDecoder & RustedAbstractDecoder::operator >> (std::string & p_str)
{
	size_t l_strLenght;
	*this >> l_strLenght;
	p_str.resize(l_strLenght);

	m_buffer.read(&p_str[0], l_strLenght);

	return *this;
}
