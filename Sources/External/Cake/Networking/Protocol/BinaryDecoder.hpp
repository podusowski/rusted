#pragma once

#include <vector>
#include <string>

#include "IReadBuffer.hpp"

namespace Cake
{
namespace Networking
{
namespace Protocol
{

class BinaryDecoder
{
public:
    BinaryDecoder(IReadBuffer &);

    BinaryDecoder & operator >> (int &);
    BinaryDecoder & operator >> (unsigned &);
    BinaryDecoder & operator >> (bool &);
    BinaryDecoder & operator >> (std::string &);
    BinaryDecoder & operator >> (float &);

    template <class A>
    BinaryDecoder & operator >> (std::vector<A> & vector)
    {
        unsigned size;
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
    IReadBuffer & m_buffer;
};

}
}
}

