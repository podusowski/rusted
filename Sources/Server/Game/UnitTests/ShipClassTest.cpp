#include <gtest/gtest.h>

#include "Server/UnitTests/AbstractTest.hpp"

#include "Common/Game/Object/UnitTests/ShipMock.hpp"
#include "DataBase/DataBaseNode.hpp"
#include "Game/ShipClass.hpp"

using namespace Server;
using namespace Server::Game;

class ShipClassTest : public Server::AbstractTest
{
};

TEST_F(ShipClassTest, ApplyBasic)
{
    DataBase::DataBaseNode node("class");
    Common::Game::Object::ShipMock ship;
    ShipClass shipClass(node);

    shipClass.applyTo(ship);
}
