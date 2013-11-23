#include <cstring>
#include <cmath>

#include "BinaryCoder.hpp"

using namespace Cake::Networking::Protocol;

BinaryCoder::BinaryCoder(IWriteBuffer & p_buffer) :
    m_buffer(p_buffer)
{
}

BinaryCoder & BinaryCoder::operator << (const int p_int)
{
    m_buffer.write(&p_int, sizeof(p_int));
    return *this;
}

BinaryCoder & BinaryCoder::operator << (const bool p_value)
{
    m_buffer.write(&p_value, sizeof(p_value));
    return *this;
}

BinaryCoder & BinaryCoder::operator << (const std::string & p_str)
{
    *this << unsigned(p_str.length());

    m_buffer.write(p_str.c_str(), p_str.length());
    return *this;
}

BinaryCoder & BinaryCoder::operator << (const unsigned p_value)
{
    m_buffer.write(&p_value, sizeof(p_value));
    return *this;
}

BinaryCoder & BinaryCoder::operator << (const float value)
{
    int decimal = std::round(value * 10000);
    return *this << decimal;
}

