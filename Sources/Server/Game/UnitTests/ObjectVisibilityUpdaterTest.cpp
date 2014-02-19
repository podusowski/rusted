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

MATCHER_P(MessageEqualsTo, expected, "")
{
    if (expected.getId() != arg.getId())
    {
        *result_listener << "expected id: " << expected.getId() << ", given id: " << arg.getId();
        return false;
    }

    auto castedArg = dynamic_cast<const decltype(expected)&>(arg);
    if (!(castedArg == expected))
    {
        *result_listener << "payloads doesn't match, expected " << expected << ", given: " << castedArg;
        return false;
    }

    return true;
}

TEST_F(ObjectVisibilityUpdaterTest, ObjectsVisibleByPlayer)
{
    Common::Game::PlayerMock playerMock;
    Server::Network::ConnectionMock connectionMock;

    auto ship1Mock = std::make_shared<Common::Game::Object::ShipMock>();
    EXPECT_CALL(*ship1Mock, getId()).Times(AtLeast(1)).WillRepeatedly(Return(1));
    EXPECT_CALL(*ship1Mock, getPosition()).Times(AtLeast(1)).WillRepeatedly(Return(Common::Game::Position(0, 0, 0)));
    EXPECT_CALL(*ship1Mock, getOwnerId()).Times(AtLeast(1)).WillRepeatedly(Return(PLAYER_ID));
    universe->add(ship1Mock);

    auto ship2Mock = std::make_shared<Common::Game::Object::ShipMock>();
    EXPECT_CALL(*ship2Mock, getId()).Times(AtLeast(1)).WillRepeatedly(Return(2));
    EXPECT_CALL(*ship2Mock, getPosition()).Times(AtLeast(1)).WillRepeatedly(Return(Common::Game::Position(100, 0, 0)));
    EXPECT_CALL(*ship2Mock, getOwnerId()).Times(AtLeast(1)).WillRepeatedly(Return(PLAYER_ID));
    universe->add(ship2Mock);

    EXPECT_CALL(*playerContainerMock, invokeOnPlayer(PLAYER_ID, _)).WillOnce(InvokeArgument<1>(std::ref(playerMock), std::ref(connectionMock)));

    EXPECT_CALL(connectionMock, send(
        MessageEqualsTo(Common::Messages::VisibleObjects({
            Common::Messages::ObjectId(1),
            Common::Messages::ObjectId(2)
    }))));

    ObjectVisibilityUpdater updater;
    updater.sendVisibleObjectsByPlayer(PLAYER_ID);
}

}
}
}

