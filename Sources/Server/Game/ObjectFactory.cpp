#include <functional>

#include <boost/make_shared.hpp>

#include "ObjectFactory.hpp"
#include "Common/Game/Object/Ship.hpp"
#include "Common/Game/Object/Asteroid.hpp"
#include "Server/Game/Actions/Attack.hpp"

using namespace Server::Game;

ObjectFactory::ObjectFactory(IShipClassContainer & shipClassContainer) : 
    m_shipClassContainer(shipClassContainer)
{
}

boost::shared_ptr<Common::Game::Object::ObjectBase> ObjectFactory::create(const soci::row & row)
{
    using namespace std::placeholders;

    std::string type = row.get<std::string>("type");

    if (type == "Ship")
    {
        boost::shared_ptr<Common::Game::Object::ObjectBase> object(new Common::Game::Object::Ship);

        Common::Game::Object::Ship & ship = dynamic_cast<Common::Game::Object::Ship&>(*object);

        object->setId(row.get<int>("id"));
        ship.setOwnerId(row.get<int>("owner"));
        object->setPosition(extractPosition(row));

        unsigned shipClassId = row.get<int>("class");
        auto & shipClass = m_shipClassContainer.getById(shipClassId);
        shipClass.applyTo(ship);
        ship.setClass(shipClassId);
        ship.visitCargoHold([&](Common::Game::Object::CargoHold & cargoHold) -> void
        {
            fillCargoHold(row, cargoHold);
        });

        return object;
    }
    else if (type == "Asteroid")
    {
        boost::shared_ptr<Common::Game::Object::ObjectBase> object(new Common::Game::Object::Asteroid);

        object->setId(row.get<int>("id"));
        object->setModel(row.get<std::string>("model"));
        object->setPosition(extractPosition(row));
        object->setIntegrity(row.get<int>("integrity"));

        // set unlimited capacity for asteroids
        object->visitCargoHold([&](Common::Game::Object::CargoHold & cargoHold) -> void
        {
            fillCargoHold(row, cargoHold);
            cargoHold.setCapacity(99999999);
        });

        return object;
    }
    else
    {
        std::stringstream ss;
        ss << "invalid object type: " << type;
        throw std::runtime_error(ss.str());
    }

    return boost::shared_ptr<Common::Game::Object::ObjectBase>();
}

boost::shared_ptr<Common::Game::Object::ObjectBase> ObjectFactory::createShip(unsigned shipClassId, unsigned ownerId)
{
    LOG_DEBUG << "Creating ship with shipClass:" << shipClassId << ", owner:" << ownerId;

    auto & shipClass = m_shipClassContainer.getById(shipClassId);

    boost::shared_ptr<Common::Game::Object::ObjectBase> object(new Common::Game::Object::Ship);
    auto & ship = dynamic_cast<Common::Game::Object::Ship&>(*object);

    object->setId(0); // to be assigned by Universe
    ship.setOwnerId(ownerId);
    shipClass.applyTo(ship);
    ship.setClass(shipClassId);

    return object;
}

Common::Game::Position ObjectFactory::extractPosition(const soci::row & row)
{
    Common::Game::Position position;
    position.setX(row.get<int>("x"));
    position.setY(row.get<int>("y"));
    position.setZ(row.get<int>("z"));
    return position;
}

void ObjectFactory::fillCargoHold(const soci::row & row, Common::Game::Object::CargoHold & cargoHold)
{
    LOG_DEBUG << "Filling cargoHold";

    cargoHold.setCarbon(row.get<int>("carbon"));
    cargoHold.setHelium(row.get<int>("helium"));
}

