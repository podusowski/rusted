#pragma once

#include <memory>
#include <soci.h>

#include "Common/Game/Object/ObjectBase.hpp"

namespace Server
{
namespace Game
{

class IUniverseDataBaseFacade
{
public:
    virtual std::shared_ptr<Common::Game::Object::ObjectBase> createShip(unsigned shipClass, unsigned ownerId) = 0;
    virtual void loadObjects() = 0;

    virtual ~IUniverseDataBaseFacade() {}
};

}
}

