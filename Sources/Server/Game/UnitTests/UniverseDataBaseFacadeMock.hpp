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
    MOCK_METHOD1(create, boost::shared_ptr<Common::Game::Object::ObjectBase>(const soci::row &));
    MOCK_METHOD2(createShip, boost::shared_ptr<Common::Game::Object::ObjectBase>(unsigned, unsigned));
};

}
}
