#pragma once

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
    void load(Common::Game::Universe &, Server::DataBase::DataBase &);

private:
    Cake::DependencyInjection::Inject<IObjectFactory> m_objectFactory;
};

}
}
