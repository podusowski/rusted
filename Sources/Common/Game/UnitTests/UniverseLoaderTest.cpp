#include <gtest/gtest.h>

#include "DataBase/DataBase.hpp"
#include "Game/UniverseLoader.hpp"
#include "Game/Universe.hpp"
#include "Game/Object/Ship.hpp"

using namespace testing;

TEST(UniverseLoaderTest, LoadShip)
{
    Common::DataBase::DataBase db;
    Common::DataBase::DataBaseNode & objects = db.getRoot().createChild("objects");

    Common::DataBase::DataBaseNode & ship1 = objects.createChild("object");
    ship1.setValue("id", 1);
    ship1.setValue("type", "Ship");

    Common::Game::Universe universe;

    Common::Game::UniverseLoader loader;
    loader.load(universe, db);

    Common::Game::Object::ObjectBase & object = universe.getById<Common::Game::Object::Ship>(1);

    ASSERT_EQ(typeid(Common::Game::Object::Ship), typeid(object));
}
