#include <gtest/gtest.h>

#include "Game/Object/ObjectFactory.hpp"
#include "Game/Object/Ship.hpp"
#include "DataBase/DataBaseNode.hpp"

TEST(ObjectFactoryTest, TestLoadShip)
{
    Common::DataBase::DataBaseNode node("object");
    node.setValue("type", "Ship");
    Common::Game::Object::ObjectFactory factory;

    boost::shared_ptr<Common::Game::Object::ObjectBase> object = factory.create(node);

    ASSERT_EQ(typeid(*object), typeid(Common::Game::Object::Ship));
}
