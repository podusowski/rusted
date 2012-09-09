#include "UniverseLoader.hpp"

using namespace Server::Game;

void UniverseLoader::load(Common::Game::Universe & universe, Server::DataBase::DataBase & db)
{
    auto & objects = db.getRoot().getFirstChild("objects");

    for (auto it = objects.getChilds().begin(); it != objects.getChilds().end(); it++)
    {
        Server::DataBase::DataBaseNode & node = **it;
        universe.add(m_objectFactory->create(node));
    }
}
