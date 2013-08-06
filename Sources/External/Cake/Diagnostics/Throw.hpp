#pragma once

#include <sstream>
#include <stdexcept>

namespace Cake
{
namespace Diagnostics
{

template<class ExceptionType = std::runtime_error> class Throw
{
public:
    ~Throw()
    {
        throw std::runtime_error(m_ss.str());
    }

    template <class T> Throw<ExceptionType> & operator << (const T & v)
    {
        m_ss << v;
        return *this;
    }

private:
    std::stringstream m_ss;
};

}
}

#define THROW(ExceptionType) Cake::Diagnostics::Throw<ExceptionType>()

