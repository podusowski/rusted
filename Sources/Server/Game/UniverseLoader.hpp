#pragma once

#include <soci.h>

#include "Game/Universe.hpp"
#include "DataBase/DataBase.hpp"
#include "Game/IObjectFactory.hpp"

namespace Server
{
namespace Game
{

class UniverseLoader
{
public:
    void load(Common::Game::Universe &);
    void save(Common::Game::Universe &);

private:
    Cake::DependencyInjection::Inject<IObjectFactory> m_objectFactory;
    Cake::DependencyInjection::Inject<soci::session> m_sociSession;
};

}
}
