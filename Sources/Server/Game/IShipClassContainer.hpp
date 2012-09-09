#pragma once

#include "ShipClass.hpp"

namespace Server
{
namespace Game
{

class IShipClassContainer
{
public:
    virtual ShipClass & getById(unsigned) = 0;

    virtual ~IShipClassContainer() {}
};

}
}
