#pragma once

#include "Common/Game/Object/Ship.hpp"
#include "DataBase/DataBaseNode.hpp"

namespace Server
{
namespace Game
{

struct AvailableAction
{
    unsigned type;
    unsigned parameter;
};

class IShipClass
{
public:
    virtual void applyTo(Common::Game::Object::Ship &) = 0;
    virtual std::vector<AvailableAction> getAvailableActions() = 0;
    virtual unsigned getId() = 0;
    virtual std::string getName() = 0;

    virtual ~IShipClass() {}
};

}
}

