#include <gtest/gtest.h>

#include "Game/Object/ObjectFactory.hpp"
#include "Game/Object/Ship.hpp"
#include "DataBase/DataBaseNode.hpp"

TEST(ObjectFactoryTest, TestLoadShip)
{
    Common::DataBase::DataBaseNode node("object");
    node.setValue("type", "Ship");
    node.setValue("id", 1);
    node.setValue("owner", 2);
    node.setValue("x", 3);
    node.setValue("y", 4);
    node.setValue("z", 5);
    Common::Game::Object::ObjectFactory factory;

    boost::shared_ptr<Common::Game::Object::ObjectBase> object = factory.create(node);

    ASSERT_EQ(typeid(Common::Game::Object::Ship), typeid(*object));
    ASSERT_EQ(1, object->getId());
    ASSERT_EQ(2, dynamic_cast<Common::Game::Object::OwnedObjectBase&>(*object).getOwnerId());

    Common::Game::Position position = object->getPosition();
    ASSERT_EQ(3, position.getX());
    ASSERT_EQ(4, position.getY());
    ASSERT_EQ(5, position.getZ());
}
