#include "Cargo.hpp"

using namespace Common::Game::Object;

Cargo::Cargo() : m_helium(0), m_carbon(0)
{
}

unsigned Cargo::getHelium()
{
    return m_helium;
}

void Cargo::setHelium(unsigned value)
{
    m_helium = value;
}

unsigned Cargo::getCarbon()
{
    return m_carbon;
}

void Cargo::setCarbon(unsigned value)
{
    m_carbon = value;
}

