#include "ObjectFactory.hpp"
#include "Ship.hpp"

using namespace Common::Game::Object;

boost::shared_ptr<IObject> ObjectFactory::create(Common::DataBase::DataBaseNode & data)
{
    if (data.getValue<std::string>("type") == "Ship")
    {
        boost::shared_ptr<IObject> object(new Ship);
        return object;
    }
    return boost::shared_ptr<IObject>();
}
