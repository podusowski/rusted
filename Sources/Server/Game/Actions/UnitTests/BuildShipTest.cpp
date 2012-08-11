#include <gtest/gtest.h>

#include "Server/Game/UnitTests/PlayerMock.hpp"
#include "Common/Game/Object/Ship.hpp"
#include "Game/Actions/BuildShip.hpp"

using namespace testing;

TEST(BuildShipTest, Build)
{
    Common::Game::Universe universe; 
    Server::Game::PlayerMock player;

    ON_CALL(player, getId()).WillByDefault(Return(1));

    Server::Game::Actions::BuildShip buildShip(universe, player);
    buildShip.execute();

    auto ships = universe.get<Common::Game::Object::Ship>();

    ASSERT_FALSE(ships.empty());

    auto & ship = dynamic_cast<Common::Game::Object::Ship &>(*ships.at(0));
    EXPECT_EQ(player.getId(), ship.getOwnerId());
}

