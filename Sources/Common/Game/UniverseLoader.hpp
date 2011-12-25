#pragma once

#include "Game/Universe.hpp"
#include "DataBase/DataBase.hpp"

namespace Common
{
namespace Game
{

class UniverseLoader
{
public:
    void load(Common::Game::Universe &, Common::DataBase::DataBase &);
};

}
}
