#include <functional>

#include <boost/make_shared.hpp>

#include "ObjectFactory.hpp"
#include "Common/Game/Object/Ship.hpp"
#include "Common/Game/Object/Asteroid.hpp"
#include "Server/Game/Actions/Attack.hpp"

using namespace Server::Game;

ObjectFactory::ObjectFactory(DataBase::DataBase & db, IShipClassContainer & shipClassContainer) : 
    m_db(db),
    m_shipClassContainer(shipClassContainer)
{
}

boost::shared_ptr<Common::Game::Object::ObjectBase> ObjectFactory::create(Server::DataBase::DataBaseNode & data)
{
    using namespace std::placeholders;

    std::string type = data.getValue<std::string>("type");

    if (type == "Ship")
    {
        boost::shared_ptr<Common::Game::Object::ObjectBase> object(new Common::Game::Object::Ship);

        Common::Game::Object::Ship & ship = dynamic_cast<Common::Game::Object::Ship&>(*object);

        object->setId(data.getValue<unsigned>("id"));
        ship.setOwnerId(data.getValue<unsigned>("owner"));
        object->setPosition(extractPosition(data));

        unsigned shipClassId = data.getValue<unsigned>("class");
        auto & shipClass = m_shipClassContainer.getById(shipClassId);
        shipClass.applyTo(ship);
        ship.setClass(shipClassId);
        ship.visitCargoHold(std::bind(&ObjectFactory::fillCargoHold, this, data, _1));

        return object;
    }
    else if (type == "Asteroid")
    {
        boost::shared_ptr<Common::Game::Object::ObjectBase> object(new Common::Game::Object::Asteroid);

        object->setId(data.getValue<unsigned>("id"));
        object->setPosition(extractPosition(data));
        object->setIntegrity(data.getValue<unsigned>("integrity"));
        object->visitCargoHold(std::bind(&ObjectFactory::fillCargoHold, this, data, _1));

        // set unlimited capacity for asteroids
        object->visitCargoHold([](Common::Game::Object::CargoHold & cargoHold) -> void
        {
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

Common::Game::Position ObjectFactory::extractPosition(Server::DataBase::DataBaseNode & data)
{
    Common::Game::Position position;
    position.setX(data.getValue<int>("x"));
    position.setY(data.getValue<int>("y"));
    position.setZ(data.getValue<int>("z"));
    return position;
}

void ObjectFactory::fillCargoHold(DataBase::DataBaseNode & node, Common::Game::Object::CargoHold & cargoHold)
{
    LOG_DEBUG << "Filling cargoHold";

    cargoHold.setCarbon(node.getValue<unsigned>("carbon", 0));
    cargoHold.setHelium(node.getValue<unsigned>("helium", 0));
}

