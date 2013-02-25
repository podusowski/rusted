#include <gtest/gtest.h>

#include "Server/UnitTests/AbstractTest.hpp"
#include "DataBase/DataBase.hpp"
#include "Game/ShipClassContainer.hpp"

using namespace Server;
using namespace Server::Game;

class ShipClassContainerTest : public Server::AbstractTest
{
};

TEST_F(ShipClassContainerTest, LoadShipClasses)
{
    DataBase::DataBase db;
    auto & shipClassesNode = db.getRoot().createChild("ship_classes");
    auto & shipClassNode = shipClassesNode.createChild("ship_class");
    shipClassNode.setValue("id", 1);
    shipClassNode.setValue("name", "ship1");
    shipClassNode.setValue("speed", 1);
    shipClassNode.setValue("integrity", 1);
    shipClassNode.setValue("mesh", "mesh");

    ShipClassContainer shipClassContainer(db);

    shipClassContainer.getById(1);
    EXPECT_ANY_THROW(shipClassContainer.getById(2));
}

TEST_F(ShipClassContainerTest, DuplicatedShipClasses)
{
    DataBase::DataBase db;
    auto & shipClassesNode = db.getRoot().createChild("ship_classes");

    auto & shipClass1Node = shipClassesNode.createChild("ship_class");
    shipClass1Node.setValue("id", 1);
    shipClass1Node.setValue("name", "ship1");

    auto & shipClass2Node = shipClassesNode.createChild("ship_class");
    shipClass2Node.setValue("id", 1);
    shipClass2Node.setValue("name", "ship2");

    EXPECT_ANY_THROW(ShipClassContainer shipClassContainer(db));
}
