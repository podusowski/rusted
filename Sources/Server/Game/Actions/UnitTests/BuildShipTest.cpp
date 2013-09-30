#include <gtest/gtest.h>

#include "Server/UnitTests/AbstractTest.hpp"

#include "Common/Game/UnitTests/PlayerMock.hpp"
#include "Common/Game/Object/UnitTests/ShipMock.hpp"
#include "Common/Game/Object/Ship.hpp"

#include "Server/Network/UnitTests/ConnectionMock.hpp"
#include "Server/Game/UnitTests/PlayerContainerMock.hpp"
#include "Server/Game/UnitTests/ShipClassMock.hpp"
#include "Game/Actions/BuildShip.hpp"

using namespace testing;

class BuildShipTest : public Server::AbstractTest
{
};

TEST_F(BuildShipTest, Build)
{
    // TODO: since Universe is mostly templated, it's impossible to create nice mock for it
    //       and I'm unable to test reserveId() for example
    Common::Game::Universe universe; 
    Common::Game::PlayerMock player;
    Common::Game::Object::ShipMock focusedShip;
    std::shared_ptr<Common::Game::Object::ObjectBase> createdObject(new Common::Game::Object::ShipMock);
    Server::Network::ConnectionMock connection;
    Server::Game::PlayerContainerMock playerContainer;
    Server::Game::ShipClassMock shipClass;
    ON_CALL(getShipClassContainerMock(), getById(_)).WillByDefault(ReturnRef(shipClass));

    Common::Game::Position focusedShipPosition(10, 20, 30);
    ON_CALL(focusedShip, getPosition()).WillByDefault(Return(focusedShipPosition));
    ON_CALL(focusedShip, getOrientation()).WillByDefault(Return(Common::Math::Quaternion()));

    ON_CALL(player, getFocusedObject()).WillByDefault(ReturnRef(focusedShip));

    std::vector<Server::Network::IConnection *> allConnections{&connection};
    ON_CALL(playerContainer, getAllConnections(_)).WillByDefault(Return(allConnections));
    ON_CALL(playerContainer, getConnectionById(_)).WillByDefault(ReturnRef(connection));

    auto & createdShip = dynamic_cast<Common::Game::Object::ShipMock&>(*createdObject);

    // this stuff is called by utilities to send new ship's info to clients
    ON_CALL(createdShip, getPosition()).WillByDefault(Return(Common::Game::Position()));
    ON_CALL(createdShip, getOrientation()).WillByDefault(Return(Common::Math::Quaternion()));
    ON_CALL(createdShip, getTrajectoryDescription()).WillByDefault(Return(Common::Game::Object::IFlightTrajectory::Description()));

    // newly created ship is flying a bit from the creator ship
    EXPECT_CALL(createdShip, setCourse(_)).Times(1);

    EXPECT_CALL(getUniverseDataBaseFacadeMock(), createShip(5, _)).Times(1).WillOnce(Return(createdObject));

    // one for newly created object and other for focused object
    EXPECT_CALL(connection, send(
                    Property(&Common::Messages::AbstractMessage::getId, Eq(Common::Messages::Id::ObjectCargoInfo))
                )).Times(2);

    EXPECT_CALL(connection, send(
                    Property(&Common::Messages::AbstractMessage::getId, Eq(Common::Messages::Id::ShipInfo))
                )).Times(1);

    EXPECT_CALL(connection, send(
                    Property(&Common::Messages::AbstractMessage::getId, Eq(Common::Messages::Id::ShipCourseInfo))
                )).Times(1);

    ON_CALL(player, getId()).WillByDefault(Return(1));

    Server::Game::Actions::BuildShip buildShip(universe, player, playerContainer, 5);
    buildShip.start();
    buildShip.finish();

    auto ships = universe.get<Common::Game::Object::Ship>();

    // hack for gmock bug: http://code.google.com/p/googlemock/issues/detail?id=79
    testing::Mock::VerifyAndClear(&getUniverseDataBaseFacadeMock());
}

