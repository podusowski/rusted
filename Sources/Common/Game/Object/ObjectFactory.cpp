#include "ObjectFactory.hpp"

using namespace Common::Game::Object;

boost::shared_ptr<IObject> ObjectFactory::createObject(Common::DataBase::DataBaseNode & data)
{
    return boost::shared_ptr<IObject>();
}
