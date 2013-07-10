#include "UniverseLoader.hpp"

using namespace Server::Game;

void UniverseLoader::load(Common::Game::Universe & universe)
{
    LOG_INFO << "Loading universe state from db";

    int objectCount;
    m_sociSession->once << "SELECT COUNT(*) FROM objects", soci::into(objectCount);
    LOG_INFO << "  " << objectCount << " objects";

    soci::rowset<soci::row> objects = (m_sociSession->prepare << "SELECT * FROM objects");

    for (auto it = objects.begin(); it != objects.end(); it++)
    {
        universe.add(m_objectFactory->create(*it));
    }
}

void UniverseLoader::save(Common::Game::Universe & universe)
{
    LOG_INFO << "Saving universe state from db";
    LOG_ERR << "Not implemented";
}
