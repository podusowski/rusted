#pragma once

#include <string>
#include <vector>

#include "IWriteBuffer.hpp"

namespace Cake
{
namespace Networking
{
namespace Protocol
{

class BinaryCoder
{
public:
    BinaryCoder(IWriteBuffer &);

    BinaryCoder & operator << (const int p_int);
    BinaryCoder & operator << (const unsigned);
    BinaryCoder & operator << (const bool p_value);
    BinaryCoder & operator << (const std::string & p_str);

    template <class A>
    BinaryCoder & operator << (const std::vector<A> & vector)
    {
        *this << vector.size();

        for (typename std::vector<A>::const_iterator it = vector.begin();
                it != vector.end(); it++)
        {
            it->serialize(m_buffer);
        }
        return *this;
    }

private:
    IWriteBuffer & m_buffer;
};

}
}
}

