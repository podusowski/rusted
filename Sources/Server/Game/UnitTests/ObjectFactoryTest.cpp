#include <gtest/gtest.h>

#include "Server/UnitTests/AbstractTest.hpp"

#include "Game/ObjectFactory.hpp"
#include "Common/Game/Object/Ship.hpp"
#include "Common/Game/Object/StaticObject.hpp"
#include "DataBase/DataBaseNode.hpp"
#include "DataBase/DataBase.hpp"
#include "Game/UnitTests/ShipClassContainerMock.hpp"

using namespace testing;

class ObjectFactoryTest : public Server::AbstractTest
{
public:
    void SetUp()
    {
        ON_CALL(getTimeMock(), getCurrentTime()).WillByDefault(Return(Common::Game::TimeValue()));
    }
};

TEST_F(ObjectFactoryTest, TestLoadShip)
{
    Server::DataBase::DataBase db;

    auto & node = db.getRoot().createChild("object");

    node.setValue("type", "Ship");
    node.setValue("id", 1);
    node.setValue("owner", 2);
    node.setValue("x", 3);
    node.setValue("y", 4);
    node.setValue("z", 5);
    node.setValue("integrity", 100);
    node.setValue("class", 1);

    auto & shipClasses = db.getRoot().createChild("ship_classes");
    auto & shipClass = shipClasses.createChild("ship_class");

    shipClass.setValue("id", 1);
    shipClass.setValue("speed", 10);

    Server::Game::ShipClassContainerMock shipClassContainer;
    Server::Game::ObjectFactory factory(db, shipClassContainer);

    boost::shared_ptr<Common::Game::Object::ObjectBase> object = factory.create(node);

    ASSERT_EQ(typeid(Common::Game::Object::Ship), typeid(*object));
    ASSERT_EQ(1, object->getId());
    ASSERT_EQ(2, dynamic_cast<Common::Game::Object::OwnedObjectBase&>(*object).getOwnerId());

    Common::Game::Position position = object->getPosition();
    ASSERT_EQ(3, position.getX());
    ASSERT_EQ(4, position.getY());
    ASSERT_EQ(5, position.getZ());

    ASSERT_EQ(100, object->getIntegrity());

    auto & ship = dynamic_cast<Common::Game::Object::Ship &>(*object);
    ASSERT_EQ(10, ship.getSpeed());
}

TEST_F(ObjectFactoryTest, LoadStaticObject)
{
    Server::DataBase::DataBaseNode node("object");
    node.setValue("type", "StaticObject");
    node.setValue("id", 1);
    node.setValue("x", 3);
    node.setValue("y", 4);
    node.setValue("z", 5);
    node.setValue("integrity", 100);

    Server::DataBase::DataBase db;
    Server::Game::ShipClassContainerMock shipClassContainer;
    Server::Game::ObjectFactory factory(db, shipClassContainer);

    boost::shared_ptr<Common::Game::Object::ObjectBase> object = factory.create(node);

    ASSERT_EQ(typeid(Common::Game::Object::StaticObject), typeid(*object));
    Common::Game::Position position = object->getPosition();
    ASSERT_EQ(3, position.getX());
    ASSERT_EQ(4, position.getY());
    ASSERT_EQ(5, position.getZ());

    ASSERT_EQ(100, object->getIntegrity());
}
