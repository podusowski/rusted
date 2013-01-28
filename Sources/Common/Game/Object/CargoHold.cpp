#include "CargoHold.hpp"

using namespace Common::Game::Object;

CargoHold::CargoHold() : m_helium(0), m_carbon(0)
{
}

unsigned CargoHold::getHelium()
{
    return m_helium;
}

void CargoHold::setHelium(unsigned value)
{
    m_helium = value;
}

unsigned CargoHold::getCarbon()
{
    return m_carbon;
}

void CargoHold::setCarbon(unsigned value)
{
    m_carbon = value;
}

