#pragma once

#include <boost/shared_ptr.hpp>
#include <soci.h>

#include "Common/Game/Object/ObjectBase.hpp"

namespace Server
{
namespace Game
{

class IUniverseDataBaseFacade
{
public:
    virtual boost::shared_ptr<Common::Game::Object::ObjectBase> createShip(unsigned shipClass, unsigned ownerId) = 0;

    virtual ~IUniverseDataBaseFacade() {}
};

}
}

