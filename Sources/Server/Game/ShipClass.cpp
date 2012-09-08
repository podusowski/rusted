#include "ShipClass.hpp"

using namespace Server::Game;

ShipClass::ShipClass(DataBase::DataBaseNode & node) :
    m_id(node.getValue<unsigned>("id")),
    m_speed(node.getValue<unsigned>("speed")),
    m_integrity(node.getValue<unsigned>("integrity"))
{
}

void ShipClass::applyTo(Common::Game::Object::Ship & ship)
{
    ship.setSpeed(m_speed);
    ship.setIntegrity(m_integrity);
}

unsigned ShipClass::getId()
{
    return m_id;
}

