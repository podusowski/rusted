#include "BinaryDecoder.hpp"

using namespace Cake::Networking::Protocol;

BinaryDecoder::BinaryDecoder(IReadBuffer & buffer) :
    m_buffer(buffer)
{
}

BinaryDecoder & BinaryDecoder::operator >> (int & i)
{
    m_buffer.read(&i, sizeof(i));
    return *this;
}

BinaryDecoder & BinaryDecoder::operator >> (bool & value)
{
    m_buffer.read(&value, sizeof(value));
    return *this;
}

BinaryDecoder & BinaryDecoder::operator >> (unsigned & value)
{
    m_buffer.read(&value, sizeof(value));
    return *this;
}

BinaryDecoder & BinaryDecoder::operator >> (std::string & p_str)
{
	size_t l_strLenght;
	*this >> l_strLenght;
	p_str.resize(l_strLenght);

	m_buffer.read(&p_str[0], l_strLenght);

	return *this;
}

