#pragma once

#include "Common/Game/Object/Ship.hpp"

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
    virtual unsigned getRequiredCarbon() = 0;
    virtual unsigned getRequiredHelium() = 0;

    virtual ~IShipClass() {}
};

}
}

