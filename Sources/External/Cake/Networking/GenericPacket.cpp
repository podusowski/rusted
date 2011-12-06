#include "GenericPacket.hpp"

using namespace Cake::Networking;

GenericPacket::GenericPacket()
{
}

GenericPacket::GenericPacket(unsigned id) : m_id(id)
{
}

unsigned GenericPacket::getId() const
{
    if (m_id)
    {
        return *m_id;
    }
    else
    {
        throw std::domain_error("no id is set");
    }
}

void GenericPacket::setId(unsigned id)
{
    m_id = id;
}
