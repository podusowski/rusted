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
    node.setValue("integrity", 100);
    node.setValue("speed", 1);
    node.setValue("id", 1);

    Common::Game::Object::ShipMock ship;

    EXPECT_CALL(ship, setIntegrity(100)).Times(1);
    EXPECT_CALL(ship, setSpeed(1)).Times(1);

    ShipClass shipClass(node);
    shipClass.applyTo(ship);

    EXPECT_EQ(1u, shipClass.getId());
}

TEST_F(ShipClassTest, AvailableActions)
{
    DataBase::DataBaseNode node("class");
    node.setValue("integrity", 100);
    node.setValue("speed", 1);
    node.setValue("id", 1);

    auto & actionsNode = node.createChild("actions");
    auto & action1Node = actionsNode.createChild("action");
    action1Node.setValue("type", 1);
    action1Node.setValue("parameter", 1);

    ShipClass shipClass(node);

    auto availableActions = shipClass.getAvailableActions();

    EXPECT_EQ(1u, availableActions.size());
}

