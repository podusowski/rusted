#pragma once

#include <string>
#include <map>
#include <stdexcept>

#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>

#include "Socket.hpp"

namespace Cake
{
namespace Networking
{

class GenericPacket
{
public:
    GenericPacket();
    explicit GenericPacket(unsigned id);

    unsigned getId() const;
    void setId(unsigned id);

    template <typename T> void setValue(const std::string & name, const T & value)
    {
        m_map.insert(std::make_pair(name, boost::lexical_cast<std::string>(value)));
    }

    template <typename T> T getValue(const std::string & name) const
    {
        Socket::StringMap::const_iterator it = m_map.find(name);
        if (it != m_map.end())
        {
            return boost::lexical_cast<T>(it->second);
        }
        else
        {
            throw std::out_of_range(name);
        }
    }

    Socket::StringMap getAllData() const
    {
        return m_map;
    }

    void setAllData(const Socket::StringMap & data)
    {
        m_map = data;
    }

private:
    Socket::StringMap m_map;
    boost::optional<unsigned> m_id;
};

}
}

inline Cake::Networking::Socket & operator<<(Cake::Networking::Socket & stream, const Cake::Networking::GenericPacket & packet)
{
    stream << packet.getId() << packet.getAllData();
    return stream;
}

inline Cake::Networking::Socket & operator>>(Cake::Networking::Socket & stream, Cake::Networking::GenericPacket & packet)
{
    unsigned id;
    Cake::Networking::Socket::StringMap data;
    stream >> id >> data;

    packet.setId(id);
    packet.setAllData(data);

    return stream;
}
