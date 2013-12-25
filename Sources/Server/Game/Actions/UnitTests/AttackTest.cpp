#include <gtest/gtest.h>

#include "Server/UnitTests/AbstractTest.hpp"

#include "Common/Game/Object/UnitTests/ShipMock.hpp"
#include "Server/Network/UnitTests/ConnectionMock.hpp"
#include "Server/Game/UnitTests/PlayerContainerMock.hpp"
#include "Game/Actions/Attack.hpp"

using namespace testing;

class AttackTest : public Server::AbstractTest
{
public:
    AttackTest() :
        ship1(new Common::Game::Object::ShipMock()),
        ship2(new Common::Game::Object::ShipMock()),
        FOCUSED_OBJECT_ID(3),
        SELECTED_OBJECT_ID(4),
        ACTION_PARAMETERS(PLAYER_ID, ATTACK_ID, ATTACK_PARAMETER, FOCUSED_OBJECT_ID, SELECTED_OBJECT_ID)
    {
        ON_CALL(*ship1, getId()).WillByDefault(Return(FOCUSED_OBJECT_ID.get()));
        ON_CALL(*ship2, getId()).WillByDefault(Return(SELECTED_OBJECT_ID.get()));

        universe.add(ship1);
        universe.add(ship2);
    }

    std::shared_ptr<Common::Game::Object::ShipMock> ship1;
    std::shared_ptr<Common::Game::Object::ShipMock> ship2;
    Common::Game::Universe universe;

    static const int ATTACK_ID = 1;
    static const int ATTACK_PARAMETER = 2;
    static const int PLAYER_ID = 2;
    const Common::Game::Object::ObjectBase::StrictId FOCUSED_OBJECT_ID;
    const Common::Game::Object::ObjectBase::Id SELECTED_OBJECT_ID;
    const Server::Game::Actions::ActionParameters ACTION_PARAMETERS;
};

TEST_F(AttackTest, AttackOtherShip)
{
    Server::Network::ConnectionMock connection;
    Server::Game::PlayerContainerMock playerContainer;

    ON_CALL(*ship1, getTrajectoryDescription()).WillByDefault(Return(Common::Game::Object::IFlightTrajectory::Description()));
    ON_CALL(*ship1, getPosition()).WillByDefault(Return(Common::Game::Position(1500, 0, 0)));
    ON_CALL(*ship2, getTrajectoryDescription()).WillByDefault(Return(Common::Game::Object::IFlightTrajectory::Description()));
    ON_CALL(*ship2, getPosition()).WillByDefault(Return(Common::Game::Position()));

    std::vector<Server::Network::IConnection *> allConnections{&connection};
    EXPECT_CALL(playerContainer, getAllConnections(_)).WillRepeatedly(Return(allConnections));

    EXPECT_CALL(connection, send(
                    Property(&Common::Messages::AbstractMessage::getId, Eq(Common::Messages::Id::EmitMovingMeshEffect))
                )).Times(1);

    // doesn't matter if this is called, might be cached or something
    // the thing is what to return if it's called
    ON_CALL(*ship2, getIntegrity()).WillByDefault(Return(100));

    EXPECT_CALL(*ship2, setIntegrity(_)).Times(0);

    Server::Game::Actions::Attack attack(universe, playerContainer, ACTION_PARAMETERS);
    auto actionTime = attack.start();

    // action time will be based on the distance of two ships
    auto distance = Common::Game::Position::distance(ship1->getPosition(), ship2->getPosition());
    int weaponSpeed = 1000;
    float expectedTime = float(distance) / float(weaponSpeed);
    int expectedSeconds = floor(expectedTime);
    int expectedMiliseconds = round((expectedTime - expectedSeconds) * 100);

    EXPECT_EQ(Common::Game::TimeValue(expectedSeconds, expectedMiliseconds), actionTime);
}

TEST_F(AttackTest, AttackOtherShip_Finish)
{
    Server::Network::ConnectionMock connection;
    Server::Game::PlayerContainerMock playerContainer;

    ON_CALL(*ship1, getTrajectoryDescription()).WillByDefault(Return(Common::Game::Object::IFlightTrajectory::Description()));
    ON_CALL(*ship1, getPosition()).WillByDefault(Return(Common::Game::Position(1000, 0, 0)));
    ON_CALL(*ship2, getTrajectoryDescription()).WillByDefault(Return(Common::Game::Object::IFlightTrajectory::Description()));
    ON_CALL(*ship2, getPosition()).WillByDefault(Return(Common::Game::Position()));

    std::vector<Server::Network::IConnection *> allConnections{&connection};
    ON_CALL(playerContainer, getAllConnections(_)).WillByDefault(Return(allConnections));

    ON_CALL(*ship2, getIntegrity()).WillByDefault(Return(100));
    EXPECT_CALL(*ship2, setIntegrity(90)).Times(1);

    EXPECT_CALL(connection, send(
                    Property(&Common::Messages::AbstractMessage::getId, Eq(Common::Messages::Id::EmitExplosionEffect))
                )).Times(1);
    EXPECT_CALL(connection, send(
                    Property(&Common::Messages::AbstractMessage::getId, Eq(Common::Messages::Id::ObjectIntegrity))
                )).Times(1);

    Server::Game::Actions::Attack attack(universe, playerContainer, ACTION_PARAMETERS);
    attack.finish();
}

TEST_F(AttackTest, AttackDestroyedShip)
{
    Server::Network::ConnectionMock connection;
    Server::Game::PlayerContainerMock playerContainer;

    std::vector<Server::Network::IConnection *> allConnections{&connection};
    ON_CALL(playerContainer, getAllConnections(_)).WillByDefault(Return(allConnections));

    EXPECT_CALL(connection, send(_)).Times(0);

    ON_CALL(*ship2, getIntegrity()).WillByDefault(Return(0));

    EXPECT_CALL(*ship2, setIntegrity(_)).Times(0);

    Server::Game::Actions::Attack attack(universe, playerContainer, ACTION_PARAMETERS);

    EXPECT_FALSE(attack.isAbleToStart());

    attack.start();
}

