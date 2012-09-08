#include <stdexcept>

#include "ShipClassContainer.hpp"

using namespace Server::Game;

ShipClassContainer::ShipClassContainer(Server::DataBase::DataBase & db) : m_db(db)
{
    loadFromDataBase();
}

ShipClassContainer::~ShipClassContainer()
{
}

Server::Game::ShipClass & ShipClassContainer::getById(unsigned id)
{
    auto it = m_shipClassMap.find(id);
    if (it != m_shipClassMap.end())
    {
        return *it->second;
    }
    else
    {
        throw std::runtime_error("no such ship class");
    }
}

void ShipClassContainer::loadFromDataBase()
{
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

