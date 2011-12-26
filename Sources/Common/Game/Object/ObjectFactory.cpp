#include "ObjectFactory.hpp"
#include "Ship.hpp"

using namespace Common::Game::Object;

boost::shared_ptr<ObjectBase> ObjectFactory::create(Common::DataBase::DataBaseNode & data)
{
    if (data.getValue<std::string>("type") == "Ship")
    {
        boost::shared_ptr<ObjectBase> object(new Ship);
        object->setId(data.getValue<unsigned>("id"));
        dynamic_cast<Ship&>(*object).setOwnerId(data.getValue<unsigned>("owner"));

        Position position;
        position.setX(data.getValue<int>("x"));
        position.setY(data.getValue<int>("y"));
        position.setZ(data.getValue<int>("z"));

        object->setPosition(position);

        return object;
    }
    return boost::shared_ptr<ObjectBase>();
}
