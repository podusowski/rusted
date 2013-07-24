#include <stdexcept>
#include <boost/lexical_cast.hpp>

#include "FcDecoder.hpp"

using namespace Cake::Networking::Protocol;

FcDecoder::FcDecoder(const Cake::Serialization::Fc & fc) :
    m_fc(fc),
    m_data(m_fc.getParameters()),
    m_iterator(m_data.begin())
{
}

FcDecoder & FcDecoder::operator >> (int & value)
{
    throwIfIteratorOutOfRange();
    value = boost::lexical_cast<int>(*m_iterator);
    m_iterator++;
    return *this;
}

FcDecoder & FcDecoder::operator >> (unsigned & value)
{
    throwIfIteratorOutOfRange();
    value = boost::lexical_cast<unsigned>(*m_iterator);
    m_iterator++;
    return *this;
}

FcDecoder & FcDecoder::operator >> (bool & value)
{
    throwIfIteratorOutOfRange();
    value = boost::lexical_cast<bool>(*m_iterator);
    m_iterator++;
    return *this;
}

FcDecoder & FcDecoder::operator >> (std::string & value)
{
    throwIfIteratorOutOfRange();
    value = *m_iterator;
    m_iterator++;
    return *this;
}

FcDecoder & FcDecoder::operator >> (float & value)
{
    throwIfIteratorOutOfRange();
    value = boost::lexical_cast<float>(*m_iterator);
    m_iterator++;
    return *this;
}

void FcDecoder::throwIfIteratorOutOfRange()
{
    if (m_iterator == m_data.end())
    {
        std::stringstream ss;
        ss << "end of data, size: " << m_data.size();
        throw std::runtime_error(ss.str());
    }
}
