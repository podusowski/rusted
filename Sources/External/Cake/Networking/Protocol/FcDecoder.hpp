#pragma once

#include <stdexcept>
#include <string>
#include <vector>

#include "Cake/Serialization/Fc.hpp"

namespace Cake
{
namespace Networking
{
namespace Protocol
{

class FcDecoder
{
public:
    FcDecoder(const Cake::Serialization::Fc & fc);

    FcDecoder & operator >> (int &);
    FcDecoder & operator >> (unsigned &);
    FcDecoder & operator >> (bool &);
    FcDecoder & operator >> (std::string &);
    FcDecoder & operator >> (float &);

    template <class A>
    FcDecoder & operator >> (std::vector<A> & vector)
    {
        throw std::runtime_error("arrays are not supported by FcDecoder");
        return *this;
    }

private:
    void throwIfIteratorOutOfRange();

    const Cake::Serialization::Fc & m_fc;
    std::vector<std::string> m_data;
    std::vector<std::string>::const_iterator m_iterator;
};

}
}
}
