#include "UniverseLoader.hpp"
#include "Game/ObjectFactory.hpp"

using namespace Server::Game;

void UniverseLoader::load(Common::Game::Universe & universe, Server::DataBase::DataBase & db)
{
    auto & objects = db.getRoot().getFirstChild("objects");
    Server::Game::ObjectFactory factory(db);

    for (auto it = objects.getChilds().begin(); it != objects.getChilds().end(); it++)
    {
        Server::DataBase::DataBaseNode & node = **it;
        universe.add(factory.create(node));
    }
}
