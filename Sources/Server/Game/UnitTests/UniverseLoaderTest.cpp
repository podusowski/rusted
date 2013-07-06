#include <gtest/gtest.h>
#include <boost/shared_ptr.hpp>

#include "Server/UnitTests/AbstractTest.hpp"

#include "DataBase/DataBase.hpp"
#include "Game/UniverseLoader.hpp"
#include "Game/Universe.hpp"
#include "Game/Object/Ship.hpp"
#include "Common/Game/Object/UnitTests/ShipMock.hpp"

using namespace testing;

class UniverseLoaderTest : public Server::AbstractTest
{
};

TEST_F(UniverseLoaderTest, LoadShip)
{
    Server::DataBase::DataBase db;
    auto & objects = db.getRoot().createChild("objects");

    auto & ship1 = objects.createChild("object");
    ship1.setValue("id", 1);
    ship1.setValue("type", "Ship");
    ship1.setValue("owner", 2);
    ship1.setValue("x", 1);
    ship1.setValue("y", 1);
    ship1.setValue("z", 1);
    ship1.setValue("integrity", 100);

    Common::Game::Universe universe;

    boost::shared_ptr<Common::Game::Object::ShipMock> ship(new Common::Game::Object::ShipMock);
    ON_CALL(*ship, getId()).WillByDefault(Return(1));
    EXPECT_CALL(getObjectFactoryMock(), create(_)).Times(1).WillRepeatedly(Return(ship));

    Server::Game::UniverseLoader loader;
    loader.load(universe, db);

    Common::Game::Object::ObjectBase & object = universe.getById<Common::Game::Object::Ship>(1);
    EXPECT_EQ(ship.get(), &object);

    // hack for gmock bug: http://code.google.com/p/googlemock/issues/detail?id=79
    testing::Mock::VerifyAndClear(&getObjectFactoryMock());
}

