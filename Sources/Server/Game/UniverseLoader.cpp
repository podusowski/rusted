#include "UniverseLoader.hpp"

using namespace Server::Game;

void UniverseLoader::load(Common::Game::Universe & universe)
{
    m_objectFactory->loadFromDb();
}

void UniverseLoader::save(Common::Game::Universe & universe)
{
    LOG_INFO << "Saving universe state from db";
    LOG_ERR << "Not implemented";
}
