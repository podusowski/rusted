#include "ObjectFactory.hpp"
#include "Ship.hpp"

using namespace Common::Game::Object;

boost::shared_ptr<ObjectBase> ObjectFactory::create(Common::DataBase::DataBaseNode & data)
{
    if (data.getValue<std::string>("type") == "Ship")
    {
        boost::shared_ptr<ObjectBase> object(new Ship);
        return object;
    }
    return boost::shared_ptr<ObjectBase>();
}
