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
        RustedAbstractDecoder & operator >> (std::vector<boost::tuple<A> > & vector)
        {
            size_t size;
            *this >> size;
            while (size-- > 0)
            {
                A value;
                *this >> value;
                vector.push_back(boost::make_tuple(value));
            }
            return *this;
        }

        template <class A, class B>
        RustedAbstractDecoder & operator >> (std::vector<boost::tuple<A, B> > & vector)
        {
            size_t size;
            *this >> size;
            while (size-- > 0)
            {
                A valueA;
                B valueB;
                *this >> valueA >> valueB;
                vector.push_back(boost::make_tuple(valueA, valueB));
            }
            return *this;
        }

        template <class A, class B, class C>
        RustedAbstractDecoder & operator >> (std::vector<boost::tuple<A, B, C> > & vector)
        {
            size_t size;
            *this >> size;
            while (size-- > 0)
            {
                A valueA;
                B valueB;
                C valueC;
                *this >> valueA >> valueB >> valueC;
                vector.push_back(boost::make_tuple(valueA, valueB, valueC));
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
