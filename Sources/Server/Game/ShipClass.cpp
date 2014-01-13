#include "Cake/Diagnostics/Logger.hpp"

#include "ShipClass.hpp"

using namespace Server::Game;

ShipClass::ShipClass(const soci::row & row) :
    m_id(row.get<int>("id")),
    m_name(row.get<std::string>("name")),
    m_speed(row.get<int>("speed")),
    m_integrity(row.get<int>("max_integrity")),
    m_model(row.get<std::string>("model")),
    m_capacity(row.get<int>("max_capacity")),
    m_requiredCarbon(row.get<int>("required_carbon")),
    m_requiredHelium(row.get<int>("required_helium"))
{
    LOG_DEBUG << "Ship class loaded, id: " << m_id 
              << ", name: " << m_name
              << ", speed: " << m_speed 
              << ", integrity: " << m_integrity
              << ", model: " << m_model
              << ", capacity: " << m_capacity;

    try
    {
        LOG_DEBUG << "Loading available actions";

        soci::rowset<soci::row> actions = (m_sociSession->prepare << "SELECT * FROM ship_classes_actions WHERE ship_class_id=:id", soci::use(m_id));

        for (auto it = actions.begin(); it != actions.end(); it++)
        {
            AvailableAction action;
            action.type = it->get<int>("action_id");
            action.parameter = it->get<int>("action_parameter");
            m_availableActions.push_back(action);

            LOG_DEBUG << "  available action: " << action.type << ", parameter: " << action.parameter;
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

    ship.setMaxSpeed(m_speed);
    ship.setIntegrity(m_integrity);
    ship.setModel(m_model);
    ship.writeCargoHold([&](Common::Game::Object::CargoHold & cargoHold) -> void
    {
        cargoHold.setCapacity(m_capacity);
    });
}

std::vector<AvailableAction> ShipClass::getAvailableActions()
{
    return m_availableActions;
}

unsigned ShipClass::getId()
{
    return m_id;
}

std::string ShipClass::getName()
{
    return m_name;
}

unsigned ShipClass::getRequiredCarbon()
{
    return m_requiredCarbon;
}

unsigned ShipClass::getRequiredHelium()
{
    return m_requiredHelium;
}

