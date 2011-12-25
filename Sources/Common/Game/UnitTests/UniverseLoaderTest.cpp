#include <gtest/gtest.h>

#include "DataBase/DataBase.hpp"

using namespace testing;

TEST(UniverseLoaderTest, LoadShip)
{
    Common::DataBase::DataBase db;
    Common::DataBase::DataBaseNode & objects = db.getRoot().createChild("objects");

    Common::DataBase::DataBaseNode & ship1 = objects.createChild("object");
    ship1.setValue("id", 1);
    ship1.setValue("type", "Ship");
}
