#include "Cake/Diagnostics/Logger.hpp"

#include "ShipClass.hpp"

using namespace Server::Game;

ShipClass::ShipClass(DataBase::DataBaseNode & node) :
    m_id(node.getValue<unsigned>("id")),
    m_speed(node.getValue<unsigned>("speed")),
    m_integrity(node.getValue<unsigned>("integrity"))
{
    LOG_DEBUG << "Ship class loaded, id: " << m_id;

    try
    {
        LOG_DEBUG << "Loading available actions";

        for (auto & actionNode: node.getFirstChild("actions").getChilds())
        {
            AvailableAction action;
            action.type = actionNode->getValue<unsigned>("type");
            m_availableActions.push_back(action);

            LOG_DEBUG << "  available action: " << action.type;
        }
    }
    catch (std::exception & ex)
    {
        LOG_WARN << "Couldn't load available actions, reason: " << ex.what();
    }
}

void ShipClass::applyTo(Common::Game::Object::Ship & ship)
{
    LOG_DEBUG << "Applying class:" << m_id << " to ship:" << ship.getId();

    ship.setSpeed(m_speed);
    ship.setIntegrity(m_integrity);
}

std::vector<AvailableAction> ShipClass::getAvailableActions()
{
    return m_availableActions;
}

unsigned ShipClass::getId()
{
    return m_id;
}

