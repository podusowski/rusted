#include <gtest/gtest.h>

#include "Server/Game/UnitTests/PlayerMock.hpp"
#include "Server/Network/UnitTests/ConnectionMock.hpp"
#include "Server/Game/UnitTests/PlayerContainerMock.hpp"
#include "Common/Game/Object/Ship.hpp"
#include "Game/Actions/BuildShip.hpp"
#include "Common/Game/Object/UnitTests/ShipMock.hpp"

using namespace testing;

TEST(BuildShipTest, Build)
{
    // TODO: since Universe is mostly templated, it's impossible to create nice mock for it
    //       and I'm unable to test reserveId() for example
    Common::Game::Universe universe; 
    Server::Game::PlayerMock player;
    Common::Game::Object::ShipMock focusedShip;
    Server::Network::ConnectionMock connection;
    Server::Game::PlayerContainerMock playerContainer;

    Common::Game::Object::Course focusedShipCourse;
    ON_CALL(focusedShip, getCourse()).WillByDefault(Return(focusedShipCourse));

    std::vector<Server::Network::IConnection *> allConnections{&connection};
    ON_CALL(playerContainer, getAllConnections(_)).WillByDefault(Return(allConnections));

    EXPECT_CALL(connection, send(
                    Property(&Common::Messages::AbstractMessage::getId, Eq(Common::Messages::Id::ShipInfo))
                )).Times(1);

    EXPECT_CALL(connection, send(
                    Property(&Common::Messages::AbstractMessage::getId, Eq(Common::Messages::Id::ShipCourseInfo))
                )).Times(1);

    ON_CALL(player, getId()).WillByDefault(Return(1));

    Server::Game::Actions::BuildShip buildShip(universe, player, playerContainer);
    buildShip.execute();

    auto ships = universe.get<Common::Game::Object::Ship>();

    ASSERT_FALSE(ships.empty());

    auto & ship = dynamic_cast<Common::Game::Object::Ship &>(*ships.at(0));
    EXPECT_EQ(player.getId(), ship.getOwnerId());

    // we want new ship to fly a bit further from focused ship
    Common::Game::Object::Course newShipCourse = ship.getCourse();

    EXPECT_EQ(focusedShipCourse.start.getX(), newShipCourse.start.getX());
}

