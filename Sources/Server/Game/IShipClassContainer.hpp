#pragma once

#include "IShipClass.hpp"

namespace Server
{
namespace Game
{

class IShipClassContainer
{
public:
    virtual IShipClass & getById(unsigned) const = 0;

    virtual ~IShipClassContainer() {}
};

}
}
