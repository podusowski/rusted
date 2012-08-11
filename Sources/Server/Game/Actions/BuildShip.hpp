#pragma once

#include "Server/Game/Actions/IAction.hpp"

namespace Server
{
namespace Game
{
namespace Actions
{

class BuildShip : public IAction
{
public:
    BuildShip(Player &);
    void execute();
};

}
}
}
