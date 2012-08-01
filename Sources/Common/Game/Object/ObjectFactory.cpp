#include "ObjectFactory.hpp"
#include "Ship.hpp"
#include "StaticObject.hpp"

using namespace Common::Game::Object;

boost::shared_ptr<ObjectBase> ObjectFactory::create(Common::DataBase::DataBaseNode & data)
{
    std::string type = data.getValue<std::string>("type");

    if (type == "Ship")
    {
        boost::shared_ptr<ObjectBase> object(new Ship);

        object->setId(data.getValue<unsigned>("id"));
        dynamic_cast<Ship&>(*object).setOwnerId(data.getValue<unsigned>("owner"));
        object->setPosition(extractPosition(data));
        object->setIntegrity(data.getValue<unsigned>("integrity"));

        return object;
    }
    else if (type == "StaticObject")
    {
        boost::shared_ptr<ObjectBase> object(new StaticObject);

        object->setId(data.getValue<unsigned>("id"));
        object->setPosition(extractPosition(data));
        object->setIntegrity(data.getValue<unsigned>("integrity"));

        return object;
    }

    return boost::shared_ptr<ObjectBase>();
}

Common::Game::Position ObjectFactory::extractPosition(Common::DataBase::DataBaseNode & data)
{
    Common::Game::Position position;
    position.setX(data.getValue<int>("x"));
    position.setY(data.getValue<int>("y"));
    position.setZ(data.getValue<int>("z"));
    return position;
}
