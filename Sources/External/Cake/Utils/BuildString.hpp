#pragma once

#include <sstream>
#include <typeinfo>

namespace Cake
{
namespace Utils
{

class BuildString
{
public:
    template<class T> BuildString & operator << (const T & val)
    {
        m_ss << val;
        return *this;
    }

    operator std::string() const
    {
        return m_ss.str();
    }

private:
    std::stringstream m_ss;
};

#define BUILD_STRING Cake::Utils::BuildString()

template<class Type>
auto typeName() -> std::string
{
    return typeid(Type).name();
}

template<class Type>
auto makeString(Type value) -> std::string
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

template<class Type, class... Rest>
auto makeString(Type value, Rest... rest) -> std::string
{
    std::stringstream ss;
    ss << value << makeString(rest...);
    return ss.str();
}

}
}
