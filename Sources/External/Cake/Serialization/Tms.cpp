#include "Tms.hpp"

using namespace Cake::Serialization;

Tms::Tms()
{
}

Tms::Tms(std::istream & stream)
{
    while (stream.good())
    {
        std::string name;
        std::string value;

        stream >> name >> value;

        m_data.insert(std::make_pair(name, value));
    }
}

bool Tms::has(const std::string & name) const
{
    return m_data.find(name) != m_data.end();
}

