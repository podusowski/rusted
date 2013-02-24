#ifndef RUSTED_ABSTRACT_DECODER_HPP
#define RUSTED_ABSTRACT_DECODER_HPP

#include <vector>
#include <string>

#include <boost/tuple/tuple.hpp>

#include <Common/RustedCodec/IReadBuffer.hpp>

namespace Common
{

namespace RustedCodec
{

/*
 * @brief Rusted abstract codec, decoder part.
 */
class RustedAbstractDecoder
{
public:
	RustedAbstractDecoder(IReadBuffer & p_buffer);
	RustedAbstractDecoder & operator >> (int & p_int);
	RustedAbstractDecoder & operator >> (bool & p_value);
	RustedAbstractDecoder & operator >> (std::string & p_str);

    template <class A>
    RustedAbstractDecoder & operator >> (std::vector<A> & vector)
    {
        size_t size;
        *this >> size;
        while (size-- > 0)
        {
            A value;
            value.unserialize(m_buffer);
            vector.push_back(value);
        }
        return *this;
    }

private:
	RustedAbstractDecoder & operator >> (size_t & p_value);

	IReadBuffer & m_buffer;
};

}

}

#endif
