#ifndef RUSTED_ABSTRACT_CODER_HPP
#define RUSTED_ABSTRACT_CODER_HPP

#include <vector>
#include <string>

#include <boost/tuple/tuple.hpp>
#include <boost/foreach.hpp>

#include <Common/RustedCodec/IWriteBuffer.hpp>

namespace Common
{

namespace RustedCodec
{

/*
 * @brief Rusted abstract codec, coder part.
 */
class RustedAbstractCoder
{
public:
	RustedAbstractCoder(IWriteBuffer & p_buffer);
	RustedAbstractCoder & operator << (const int p_int);
	RustedAbstractCoder & operator << (const bool p_value);
	RustedAbstractCoder & operator << (const std::string p_str);

        template <class A>
        RustedAbstractCoder & operator << (const std::vector<boost::tuple<A> > & vector)
        {
            *this << vector.size();

            for (typename std::vector<boost::tuple<A> >::const_iterator it = vector.begin();
                    it != vector.end(); it++)
            {
                *this << boost::get<0>(*it);
            }
            return *this;
        }

        template <class A, class B>
        RustedAbstractCoder & operator << (const std::vector<boost::tuple<A, B> > & vector)
        {
            *this << vector.size();

            for (typename std::vector<boost::tuple<A, B> >::const_iterator it = vector.begin();
                    it != vector.end(); it++)
            {
                *this << boost::get<0>(*it) << boost::get<1>(*it);
            }
            return *this;
        }
private:
	RustedAbstractCoder & operator << (const size_t p_value);
	IWriteBuffer & m_buffer;
};

}

}

#endif
