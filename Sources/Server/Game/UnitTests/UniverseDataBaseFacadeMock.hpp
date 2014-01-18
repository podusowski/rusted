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
    MOCK_METHOD2(createShip, std::shared_ptr<Common::Game::Object::ObjectBase>(unsigned, unsigned));
    MOCK_METHOD0(loadObjects, void());
};

}
}

