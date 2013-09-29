#pragma once

#include <gmock/gmock.h>

#include "Game/IUniverseDataBaseFacade.hpp"

namespace Server
{
namespace Game
{

class UniverseDataBaseFacadeMock : public IUniverseDataBaseFacade
{
public:
    MOCK_METHOD2(createShip, boost::shared_ptr<Common::Game::Object::ObjectBase>(unsigned, unsigned));
};

}
}

