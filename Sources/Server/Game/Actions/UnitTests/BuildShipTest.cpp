#include <gtest/gtest.h>

#include "Game/Actions/BuildShip.hpp"

TEST(BuildShipTest, Build)
{
    Common::Game::Universe universe; 

    Server::Game::Actions::BuildShip buildShip(universe);
    buildShip.execute();
}

