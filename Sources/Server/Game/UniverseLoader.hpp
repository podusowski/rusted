#pragma once

#include "Game/Universe.hpp"
#include "DataBase/DataBase.hpp"

namespace Server
{
namespace Game
{

class UniverseLoader
{
public:
    void load(Common::Game::Universe &, Server::DataBase::DataBase &);
};

}
}
