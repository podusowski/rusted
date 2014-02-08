#include <gtest/gtest.h>

#include "Common/Game/Object/UnitTests/ShipMock.hpp"
#include "Common/Game/UnitTests/PlayerMock.hpp"

#include "Game/ObjectVisibilityUpdater.hpp"
#include "Network/UnitTests/ConnectionMock.hpp"
#include "Game/UnitTests/PlayerContainerMock.hpp"

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
    const int PLAYER_ID = 1;
}

class ObjectVisibilityUpdaterTest : public AbstractTest
{
public:
    ObjectVisibilityUpdaterTest() :
        universe(std::make_shared<Common::Game::Universe>()),
        playerContainerMock(std::make_shared<Game::PlayerContainerMock>())
    {
        r.forInterface<Common::Game::Universe>().use(universe);
        r.forInterface<Game::IPlayerContainer>().use(playerContainerMock);
    }

    Cake::DependencyInjection::Registry r;

    std::shared_ptr<Common::Game::Universe> universe;
    std::shared_ptr<Game::PlayerContainerMock> playerContainerMock;
};

TEST_F(ObjectVisibilityUpdaterTest, OneShip)
{
    Common::Game::PlayerMock playerMock;
    Server::Network::ConnectionMock connectionMock;
    auto ship1Mock = std::make_shared<Common::Game::Object::ShipMock>();
    universe->add(ship1Mock);

    EXPECT_CALL(*playerContainerMock, invokeOnPlayer(PLAYER_ID, _)).WillOnce(InvokeArgument<1>(std::ref(playerMock), std::ref(connectionMock)));
    EXPECT_CALL(*ship1Mock, getOwnerId()).WillOnce(Return(PLAYER_ID));
    EXPECT_CALL(*ship1Mock, getPosition()).WillOnce(Return(POSITION));

    EXPECT_CALL(connectionMock, send(_));

    ObjectVisibilityUpdater updater;
    updater.sendVisibleObjects(POSITION);
}

}
}
}

