#include <gtest/gtest.h>

#include "Common/Game/Object/Ship.hpp"
#include "Game/Actions/BuildShip.hpp"

TEST(BuildShipTest, Build)
{
    Common::Game::Universe universe; 

    Server::Game::Actions::BuildShip buildShip(universe);
    buildShip.execute();

    auto ships = universe.get<Common::Game::Object::Ship>();

    EXPECT_FALSE(ships.empty());
}

