#include <gtest/gtest.h>
#include <boost/shared_ptr.hpp>

#include "Server/UnitTests/AbstractTest.hpp"

#include "DataBase/DataBase.hpp"
#include "Game/UniverseLoader.hpp"
#include "Game/Universe.hpp"
#include "Game/Object/Ship.hpp"

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

    Server::Game::UniverseLoader loader;
    loader.load(universe, db);

    Common::Game::Object::ObjectBase & object = universe.getById<Common::Game::Object::Ship>(1);

    ASSERT_EQ(typeid(Common::Game::Object::Ship), typeid(object));
}
