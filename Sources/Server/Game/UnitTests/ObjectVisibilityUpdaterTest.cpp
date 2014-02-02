#include <gtest/gtest.h>

#include "Game/ObjectVisibilityUpdater.hpp"
#include "Network/UnitTests/ConnectionMock.hpp"
#include "Common/Game/Object/UnitTests/ShipMock.hpp"
#include "UnitTests/AbstractTest.hpp"

namespace Server
{
namespace Game
{
namespace Ut
{

using namespace testing;

namespace
{
    const Common::Game::Position POSITION = { 0, 0, 0 };
}

class ObjectVisibilityUpdaterTest : public AbstractTest
{
};

TEST_F(ObjectVisibilityUpdaterTest, Simple)
{
    Server::Network::ConnectionMock connectionMock;
    Common::Game::Universe universe;
    auto ship1Mock = std::make_shared<Common::Game::Object::ShipMock>();

    //EXPECT_CALL(*ship1Mock, getPosition()).WillOnce(Return(POSITION));
    universe.add(ship1Mock);

    ObjectVisibilityUpdater updater;
    updater.sendVisibleObjects(POSITION);
}

}
}
}

