#include "UniverseLoader.hpp"
#include "Object/ObjectFactory.hpp"

using namespace Common::Game;

void UniverseLoader::load(Common::Game::Universe & universe, Common::DataBase::DataBase & db)
{
    Common::DataBase::DataBaseNode & objects = db.getRoot().getFirstChild("objects");
    Common::Game::Object::ObjectFactory factory;

    for (Common::DataBase::DataBaseNode::iterator it = objects.getChilds().begin();  
         it != objects.getChilds().end(); it++)
    {
        Common::DataBase::DataBaseNode & node = **it;
        universe.add(factory.create(node));
    }
}
