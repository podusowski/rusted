#include <stdexcept>
#include <sstream>

#include "Cake/Diagnostics/Logger.hpp"
#include "ShipClassContainer.hpp"

using namespace Server::Game;

ShipClassContainer::ShipClassContainer(Server::DataBase::DataBase & db) : m_db(db)
{
    loadFromDataBase();
}

ShipClassContainer::~ShipClassContainer()
{
}

Server::Game::IShipClass & ShipClassContainer::getById(unsigned id)
{
    auto it = m_shipClassMap.find(id);
    if (it != m_shipClassMap.end())
    {
        return *it->second;
    }
    else
    {
        std::stringstream ss;
        ss << "no ship class with id:" << id;
        throw std::runtime_error(ss.str());
    }
}

void ShipClassContainer::loadFromDataBase()
{
    LOG_INFO << "Loading ship classes from db";

    auto shipClassesNode = m_db.getRoot().getFirstChild("ship_classes").getChilds();

    for (auto it = shipClassesNode.begin(); it != shipClassesNode.end(); it++)
    {
        boost::shared_ptr<ShipClass> shipClass(new ShipClass(**it));
        auto ret = m_shipClassMap.insert(std::make_pair(shipClass->getId(), shipClass));
        if (!ret.second)
        {
            throw std::runtime_error("such ShipClass already exists");
        }
    }
}

