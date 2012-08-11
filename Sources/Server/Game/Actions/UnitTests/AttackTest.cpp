#include <gtest/gtest.h>

#include "Common/Game/Object/UnitTests/ShipMock.hpp"
#include "Common/Game/UnitTests/RustedTimeStub.hpp"
#include "Server/Network/UnitTests/ConnectionMock.hpp"
#include "Server/Game/UnitTests/PlayerContainerMock.hpp"
#include "Game/Actions/Attack.hpp"

using namespace testing;

class AttackTest : public Test
{
public:
    void SetUp()
    {
        Cake::DependencyInjection::clear();
        boost::shared_ptr<Common::Game::IRustedTime> time(new RustedTimeStub);
        Cake::DependencyInjection::forInterface<Common::Game::IRustedTime>().use(time);

        ON_CALL(dynamic_cast<RustedTimeStub&>(*time), getCurrentTime()).WillByDefault(Return(Common::Game::TimeValue()));
    }
};

TEST_F(AttackTest, AttackOtherShip)
{
    Server::Network::ConnectionMock connection;
    Server::Game::PlayerContainerMock playerContainer;
    Common::Game::Object::ShipMock focusedShip;
    Common::Game::Object::ShipMock selectedShip;

    ON_CALL(focusedShip, getCourse()).WillByDefault(Return(Common::Game::Object::Course()));
    ON_CALL(selectedShip, getCourse()).WillByDefault(Return(Common::Game::Object::Course()));
    ON_CALL(selectedShip, getPosition()).WillByDefault(Return(Common::Game::Position()));

    std::vector<Server::Network::IConnection *> allConnections{&connection};
    ON_CALL(playerContainer, getAllConnections(_)).WillByDefault(Return(allConnections));

    EXPECT_CALL(connection, send(
                    Property(&Common::Messages::AbstractMessage::getId, Eq(Common::Messages::Id::AttackObject))
                )).Times(1);

    EXPECT_CALL(connection, send(
                    Property(&Common::Messages::AbstractMessage::getId, Eq(Common::Messages::Id::ShipInfo))
                )).Times(1);

    // this might be a little overhead but it's simple and might be useful later
    EXPECT_CALL(connection, send(
                    Property(&Common::Messages::AbstractMessage::getId, Eq(Common::Messages::Id::ShipCourseInfo))
                )).Times(1);

    // doesn't matter if this is called, might be cached or something
    // the thing is what to return if it's called
    ON_CALL(selectedShip, getIntegrity()).WillByDefault(Return(100));

    EXPECT_CALL(selectedShip, setIntegrity(90)).Times(1);

    Server::Game::Actions::Attack attack(playerContainer, focusedShip, selectedShip);
    attack.execute();
}

TEST_F(AttackTest, AttackDestroyedShip)
{
    Server::Network::ConnectionMock connection;
    Server::Game::PlayerContainerMock playerContainer;
    Common::Game::Object::ShipMock focusedShip;
    Common::Game::Object::ShipMock selectedShip;

    std::vector<Server::Network::IConnection *> allConnections{&connection};
    ON_CALL(playerContainer, getAllConnections(_)).WillByDefault(Return(allConnections));

    EXPECT_CALL(connection, send(_)).Times(0);

    ON_CALL(selectedShip, getIntegrity()).WillByDefault(Return(0));

    EXPECT_CALL(selectedShip, setIntegrity(_)).Times(0);

    Server::Game::Actions::Attack attack(playerContainer, focusedShip, selectedShip);
    attack.execute();
}
