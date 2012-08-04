#include <boost/make_shared.hpp>

#include "ObjectFactory.hpp"
#include "Common/Game/Object/Ship.hpp"
#include "Common/Game/Object/StaticObject.hpp"
#include "Server/Game/Actions/Attack.hpp"

using namespace Server::Game;

boost::shared_ptr<Common::Game::Object::ObjectBase> ObjectFactory::create(Server::DataBase::DataBaseNode & data)
{
    std::string type = data.getValue<std::string>("type");

    if (type == "Ship")
    {
        boost::shared_ptr<Common::Game::Object::ObjectBase> object(new Common::Game::Object::Ship);

        Common::Game::Object::Ship & ship = dynamic_cast<Common::Game::Object::Ship&>(*object);

        object->setId(data.getValue<unsigned>("id"));
        ship.setOwnerId(data.getValue<unsigned>("owner"));
        object->setPosition(extractPosition(data));
        object->setIntegrity(data.getValue<unsigned>("integrity"));

        return object;
    }
    else if (type == "StaticObject")
    {
        boost::shared_ptr<Common::Game::Object::ObjectBase> object(new Common::Game::Object::StaticObject);

        object->setId(data.getValue<unsigned>("id"));
        object->setPosition(extractPosition(data));
        object->setIntegrity(data.getValue<unsigned>("integrity"));

        return object;
    }

    return boost::shared_ptr<Common::Game::Object::ObjectBase>();
}

Common::Game::Position ObjectFactory::extractPosition(Server::DataBase::DataBaseNode & data)
{
    Common::Game::Position position;
    position.setX(data.getValue<int>("x"));
    position.setY(data.getValue<int>("y"));
    position.setZ(data.getValue<int>("z"));
    return position;
}
