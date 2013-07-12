#pragma once

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

}
}

#define BUILD_STRING Cake::Utils::BuildString()

