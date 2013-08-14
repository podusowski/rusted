#pragma once

#include <gmock/gmock.h>

#include "Game/IObjectFactory.hpp"

namespace Server
{
namespace Game
{

class ObjectFactoryMock : public IObjectFactory
{
public:
    MOCK_METHOD0(loadFromDb, void());
    MOCK_METHOD1(create, boost::shared_ptr<Common::Game::Object::ObjectBase>(const soci::row &));
    MOCK_METHOD2(createShip, boost::shared_ptr<Common::Game::Object::ObjectBase>(unsigned, unsigned));
};

}
}

